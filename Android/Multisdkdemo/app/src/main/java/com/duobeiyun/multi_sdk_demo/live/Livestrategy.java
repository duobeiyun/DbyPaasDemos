package com.duobeiyun.multi_sdk_demo.live;

import android.view.SurfaceView;

import com.duobeiyun.multi_sdk_demo.live.impl.SendMessageCallback;

public interface Livestrategy {

    //界面相关生命周期方法调用
    void resume();

    void pause();

    void destroy();

    //相关功能的实现代码

    int startLive();

    int stopLive();

    int openCamera(boolean open);

    int exchangeCamera();

    int openAudioRecord(boolean open);

    int sendMessage(String msg, SendMessageCallback callback);

    SurfaceView getSurfaceview(boolean isLocalVideo);

    int setLocalVideo(SurfaceView surfaceView);

    int setRemoteVideo(SurfaceView surfaceView, String uid);

    int setRemoteVideoStreamType(String uid, int mode);

    void setLiveCallback(LiveCallback callback);

    void setDefaultAudioRoutetoSpeakerphone(boolean speaker);

    void muteRemoteUidVideo(String username, boolean isMute);

    void muteRemoteUidAudio(String username, boolean isMute);

    void testInChannelTime(long time);

    void enableLastTest();

    void disableLastTest();

}
