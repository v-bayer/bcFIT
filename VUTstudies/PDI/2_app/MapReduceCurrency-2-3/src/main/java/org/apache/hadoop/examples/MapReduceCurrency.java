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
            extends Mapper<Object, Text, Text, Text> {

        public void map(Object key, Text value, Context context
        ) throws IOException, InterruptedException {
            StringTokenizer itr = new StringTokenizer(value.toString(), ",");
            int i = 0;
            String currency = "";
            String date = "";
            String changeRate = "";

            while (itr.hasMoreTokens()) {
                String token = itr.nextToken();

                if (i == 0) {
                    currency = token;
                } else if (i == 1) {
                    date = token.substring(0, 7);
                } else if (i == 2) {
                    changeRate = token;

                    String outputKey = currency + "," + date;
                    String outputValue = changeRate + "," + "1";

                    context.write(new Text(outputKey), new Text(outputValue));
                    break;

                }
                i++;
            }
        }
    }

    public static class IntSumReducer
            extends Reducer<Text, Text, Text, Text> {

        public void reduce(Text key, Iterable<Text> values,
                Context context
        ) throws IOException, InterruptedException {
            Double sum = 0.0;
            int count = 0;
            for (Text val : values) {
                String[] splitedValue = val.toString().split(",");
                sum += Double.parseDouble(splitedValue[0]);
                count += Integer.parseInt(splitedValue[1]);
            }
            String outputValue = sum + "," + count;
            context.write(key, new Text(outputValue));
        }
    }

    public static class FinalSumReducer
            extends Reducer<Text, Text, Text, Text> {

        public void reduce(Text key, Iterable<Text> values,
                Context context
        ) throws IOException, InterruptedException {
            Double sum = 0.0;
            int count = 0;
            for (Text val : values) {
                String[] splitedValue = val.toString().split(",");
                sum += Double.parseDouble(splitedValue[0]);
                count += Integer.parseInt(splitedValue[1]);
            }
            double avg = (double) sum / count;
            String outputValue = Double.toString(avg);
            context.write(key, new Text(outputValue));

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
        job.setCombinerClass(IntSumReducer.class);
        job.setReducerClass(FinalSumReducer.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(Text.class);

        FileInputFormat.addInputPath(job, inputFile);
        FileOutputFormat.setOutputPath(job, outputFile);

        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}
