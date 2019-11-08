package com.duobeiyun.demo_1v1.widget;

public class RecordIdBean {

    /**
     * code : 200
     * msg :
     * tag : 1
     * data : {"recordId":"8cd2fac16b6c4680990910634fd06458"}
     */

    private int code;
    private String msg;
    private int tag;
    private DataBean data;

    public int getCode() {
        return code;
    }

    public void setCode(int code) {
        this.code = code;
    }

    public String getMsg() {
        return msg;
    }

    public void setMsg(String msg) {
        this.msg = msg;
    }

    public int getTag() {
        return tag;
    }

    public void setTag(int tag) {
        this.tag = tag;
    }

    public DataBean getData() {
        return data;
    }

    public void setData(DataBean data) {
        this.data = data;
    }

    public static class DataBean {
        /**
         * recordId : 8cd2fac16b6c4680990910634fd06458
         */

        private String recordId;

        public String getRecordId() {
            return recordId;
        }

        public void setRecordId(String recordId) {
            this.recordId = recordId;
        }
    }
}
