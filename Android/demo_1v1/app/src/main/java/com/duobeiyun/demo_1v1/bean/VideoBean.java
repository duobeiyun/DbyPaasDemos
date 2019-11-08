package com.duobeiyun.demo_1v1.bean;

import android.view.SurfaceView;
import android.widget.ImageView;
import android.widget.ProgressBar;

import java.io.Serializable;

public class VideoBean implements Serializable {
    public String username;
    public SurfaceView surfaceView;
    public boolean isBig;
    public boolean hasMount;//远端视频是否显示
    public ProgressBar load;
    public ImageView micView;
    public boolean micOpen;
    public ImageView volumeView;
    public boolean isMute;//音视频是否静音

    public VideoBean(String username, SurfaceView surfaceView, boolean isBig, boolean hasMount, ProgressBar load, ImageView micView, boolean micOpen, ImageView volumeView) {
        this.username = username;
        this.surfaceView = surfaceView;
        this.isBig = isBig;
        this.hasMount = hasMount;
        this.load = load;
        this.micView = micView;
        this.micOpen = micOpen;
        this.volumeView = volumeView;
    }


}
