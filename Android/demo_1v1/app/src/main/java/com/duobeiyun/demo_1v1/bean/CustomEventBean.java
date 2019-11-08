package com.duobeiyun.demo_1v1.bean;



import androidx.annotation.Nullable;

import java.util.Objects;

public class CustomEventBean {

    /**
     * uid : 1419
     * content : 哈拆拆拆拆了
     * recordTime : 1564121826304
     */

    private String uid;
    private String content;
    private long recordTime;

    public String getUid() {
        return uid;
    }

    public void setUid(String uid) {
        this.uid = uid;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public long getRecordTime() {
        return recordTime;
    }

    public void setRecordTime(long recordTime) {
        this.recordTime = recordTime;
    }

    @Override
    public int hashCode() {
        return Objects.hash(uid, content, recordTime);
    }

    @Override
    public boolean equals(@Nullable Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        CustomEventBean that = (CustomEventBean) o;
        return recordTime == that.getRecordTime() && Objects.equals(content, that.content) && Objects.equals(uid, that.uid);
    }
}
