/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package org.apache.hadoop.examples;

import com.google.common.collect.Iterables;
import java.io.File;
import java.io.IOException;
import java.nio.file.Paths;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.InvalidPathException;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class MapReduceCurrency {

    private static Path inputFile;
    private static Path outputFile;

    public static class TokenizerMapper
            extends Mapper<Object, Text, Text, MinMaxCurrency> {

        private MinMaxCurrency outPut = new MinMaxCurrency();

        public void map(Object key, Text value, Context context
        ) throws IOException, InterruptedException {
            StringTokenizer itr = new StringTokenizer(value.toString(), ",");
            int i = 0;
            String currency = "";
            Double changeRate = 0.0;

            while (itr.hasMoreTokens()) {
                String token = itr.nextToken();

                if (i == 0) {
                    currency = token;
                } else if (i == 2) {
                    changeRate = Double.parseDouble(token);
                    outPut.setMin(changeRate);
                    outPut.setMax(changeRate);

                    context.write(new Text(currency), outPut);
                    break;

                }
                i++;
            }
        }
    }

    public static class DoubleMinMaxReducer
            extends Reducer<Text, MinMaxCurrency, Text, MinMaxCurrency> {

        private MinMaxCurrency resultRow = new MinMaxCurrency();

        public void reduce(Text key, Iterable<MinMaxCurrency> values,
                Context context
        ) throws IOException, InterruptedException {
            
            Double minCurrency = 0.0;
            Double maxCurrency = 0.0;
            int i = 0;
            Double sum = 0.0;

            resultRow.setMin(null);
            resultRow.setMax(null);

            for (MinMaxCurrency val : values) {
                minCurrency = val.getMin();
                maxCurrency = val.getMax();

                if (resultRow.getMin() == null || minCurrency.compareTo(resultRow.getMin()) < 0) {
                    resultRow.setMin(minCurrency);
                }
                if (resultRow.getMax() == null || maxCurrency.compareTo(resultRow.getMax()) > 0) {
                    resultRow.setMax(maxCurrency);
                }
                sum += minCurrency;
                i++;
            }
            double avg = (double)sum / i;
            resultRow.setAvg(avg);
            context.write(key, resultRow);
        }
    }

    public static boolean isInteger(String s) {
        boolean isValidInteger = false;
        try {
            Integer.parseInt(s);
            isValidInteger = true;
        } catch (NumberFormatException ex) {
            // s is not an integer
        }

        return isValidInteger;
    }

    public static void parseArgs(String[] args) {
        if (args.length != 2) {
            System.err.println("Incorrect arguments.");
            System.exit(2);
        }

        try {
            Paths.get(args[0]);
        } catch (InvalidPathException ex) {
            System.err.println("Input file doesn't exist.");
            System.exit(2);
        } catch (NullPointerException ex) {
            System.err.println("Input file doesn't exist.");
            System.exit(2);
        } finally {
            inputFile = new Path(args[0]);
        }
        outputFile = new Path(args[1]);
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
        parseArgs(otherArgs);

        Job job = new Job(conf, "Player team stats");
        job.setJarByClass(MapReduceCurrency.class);
        job.setMapperClass(TokenizerMapper.class);
        job.setReducerClass(DoubleMinMaxReducer.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(MinMaxCurrency.class);

        FileInputFormat.addInputPath(job, inputFile);
        FileOutputFormat.setOutputPath(job, outputFile);

        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}
