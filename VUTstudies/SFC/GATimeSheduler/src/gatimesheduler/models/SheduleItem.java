/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package gatimesheduler.models;

import gatimesheduler.BaseInfoManager;

/**
 *
 * @author Vaclav Bayer
 */
public class SheduleItem {
    
    public int id;
    private String day;
    public int dayInt;
    public int hour;
    public int subjectId;
    public int subjectOffset;
    public String subjectShortcut = "";
    public int roomId;
    public String roomShortcut = "";

    public SheduleItem(int id) {
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getDay() {
        return day;
    }

    public void setDay(String day) {
        this.day = day;
    }

    public int getDayInt() {
        return dayInt;
    }

    public void setDayInt(int dayInt) {
        this.dayInt = dayInt;
        this.day = BaseInfoManager.Days.get(dayInt);
    }

    public int getHour() {
        return hour;
    }

    public void setHour(int hour) {
        this.hour = hour;
    }

    public int getSubjectId() {
        return subjectId;
    }

    public void setSubjectId(int subjectId) {
        this.subjectId = subjectId;
    }

    public int getSubjectOffset() {
        return subjectOffset;
    }

    public void setSubjectOffset(int subjectOffset) {
        this.subjectOffset = subjectOffset;
    }

    public String getSubjectShortcut() {
        return subjectShortcut;
    }

    public void setSubjectShortcut(String subjectShortcut) {
        this.subjectShortcut = subjectShortcut;
    }

    public int getRoomId() {
        return roomId;
    }

    public void setRoomId(int roomId) {
        this.roomId = roomId;
    }

    public String getRoomShortcut() {
        return roomShortcut;
    }

    public void setRoomShortcut(String roomShortcut) {
        this.roomShortcut = roomShortcut;
    }
    
}
