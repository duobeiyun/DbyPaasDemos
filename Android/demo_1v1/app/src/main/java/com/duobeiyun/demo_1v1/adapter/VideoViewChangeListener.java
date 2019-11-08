package com.duobeiyun.demo_1v1.adapter;

import com.duobeiyun.demo_1v1.bean.VideoBean;

public interface VideoViewChangeListener {
    void onItemChange(Object item);

    void surfaceClick(VideoBean bean);
}
