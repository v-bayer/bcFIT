/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.apache.hadoop.examples;

import java.io.DataInput;
import java.io.DataOutput;
import java.io.IOException;
import org.apache.hadoop.io.Writable;

/**
 *
 * @author Vaclav Bayer
 */
public class PlayerTeamTuple implements Writable {

    private String playerID;
    private String teamID;
    private Float avgGames;
    private Integer sumGames;

    public PlayerTeamTuple(String playerID, String teamID, Float avgGames, Integer sumGames) {
        this.playerID = playerID;
        this.teamID = teamID;
        this.avgGames = avgGames;
        this.sumGames = sumGames;
    }

    public String getPlayerID() {
        return playerID;
    }

    public void setPlayerID(String playerID) {
        this.playerID = playerID;
    }

    public String getTeamID() {
        return teamID;
    }

    public void setTeamID(String teamID) {
        this.teamID = teamID;
    }

    public Float getAvgGames() {
        return avgGames;
    }

    public void setAvgGames(Float avgGames) {
        this.avgGames = avgGames;
    }

    public Integer getSumGames() {
        return sumGames;
    }

    public void setSumGames(Integer sumGames) {
        this.sumGames = sumGames;
    }
    
    public void readFields(DataInput in) throws IOException {
	playerID = in.readUTF();
	teamID = in.readUTF();
	avgGames = in.readFloat();
        sumGames = in.readInt();
    }

    public void write(DataOutput out) throws IOException {
        out.writeUTF(playerID);
        out.writeUTF(teamID);
	out.writeFloat(avgGames);
	out.writeInt(sumGames);
    }

    public String toString() {
	return playerID + "\t" + teamID + "\t" + avgGames + "\t" + sumGames;
    }

}
