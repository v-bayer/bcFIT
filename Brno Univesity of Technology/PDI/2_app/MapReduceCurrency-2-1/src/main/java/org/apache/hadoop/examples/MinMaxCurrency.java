package org.apache.hadoop.examples;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import org.apache.hadoop.io.Writable;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author Vaclav Bayer
 */
public class MinMaxCurrency implements Writable {

    Double min;
    Double max;
    Double avg;

    public MinMaxCurrency() {
        this.min = 0.0;
        this.max = 0.0;
        this.avg = 0.0;
    }

    public Double getAvg() {
        return avg;
    }

    public void setAvg(Double avg) {
        this.avg = avg;
    }

    public Double getMin() {
        return min;
    }

    public void setMin(Double min) {
        this.min = min;
    }

    public Double getMax() {
        return max;
    }

    public void setMax(Double max) {
        this.max = max;
    }

    public void write(DataOutput out) throws IOException {
        // what order we want to write !
        out.writeDouble(min);
        out.writeDouble(max);
        out.writeDouble(avg);
    }

    // readFields Method
    public void readFields(DataInput in) throws IOException {
        min = new Double(in.readDouble());
        max = new Double(in.readDouble());
        avg = new Double(in.readDouble());
    }

    public String toString() {
        return min + "," + max + "," + avg;
    }
}
