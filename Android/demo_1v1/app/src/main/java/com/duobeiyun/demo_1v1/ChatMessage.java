package com.duobeiyun.demo_1v1;

import java.io.Serializable;
import java.util.Objects;

public class ChatMessage implements Serializable, Cloneable {
    public String userId;
    public String msg;
    public int type = 1;//0:上线类型，1 聊天类型
    public boolean isOnline;//true 用户上线，false 用户下线
    public long msgTime;//发生的时间点，处理回放

    public ChatMessage(String userId, String msg) {
        this.userId = userId;
        this.msg = msg;
    }

    public ChatMessage(String userId, String msg, long msgTime) {
        this.userId = userId;
        this.msg = msg;
        this.msgTime = msgTime;
    }

    public ChatMessage(String userId, int type, boolean isOnline) {
        this.userId = userId;
        this.type = type;
        this.isOnline = isOnline;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ChatMessage that = (ChatMessage) o;
        return type == that.type &&
                isOnline == that.isOnline &&
                msgTime == that.msgTime &&
                Objects.equals(userId, that.userId) &&
                Objects.equals(msg, that.msg);
    }

    @Override
    public int hashCode() {
        return Objects.hash(userId, msg, type, isOnline, msgTime);
    }

    @Override
    protected Object clone() throws CloneNotSupportedException {
        return super.clone();
    }
}
