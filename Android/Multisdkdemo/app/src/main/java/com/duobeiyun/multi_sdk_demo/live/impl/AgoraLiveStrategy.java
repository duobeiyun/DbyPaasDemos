package com.duobeiyun.multi_sdk_demo.live.impl;

import android.content.Context;
import android.text.TextUtils;
import android.util.Log;
import android.view.SurfaceView;


import com.duobeiyun.multi_sdk_demo.R;
import com.duobeiyun.multi_sdk_demo.live.LiveCallback;
import com.duobeiyun.multi_sdk_demo.live.Livestrategy;
import com.duobeiyun.multi_sdk_demo.live.eventhandler.AgoreEventHandler;
import com.duobeiyun.multi_sdk_demo.utils.Constant;
import com.duobeiyun.multi_sdk_demo.utils.SharePreUtils;

import io.agora.rtc.RtcEngine;
import io.agora.rtc.video.VideoCanvas;
import io.agora.rtc.video.VideoEncoderConfiguration;

public class AgoraLiveStrategy implements Livestrategy {
    private RtcEngine mRtcEngine;
    private Context mContext;
    private LiveCallback liveCallback;

    public AgoraLiveStrategy(Context mContext) {
        this.mContext = mContext;

    }

    @Override
    public void setLiveCallback(LiveCallback callback) {
        this.liveCallback = callback;
        initRtcEngine();
    }

    @Override
    public void resume() {

    }

    @Override
    public void pause() {
    }

    @Override
    public void destroy() {
        RtcEngine.destroy();
    }

    @Override
    public int startLive() {
        String channelId = SharePreUtils.getInstance(mContext).getValue(Constant.CHANNELID, "123");
        String userId = SharePreUtils.getInstance(mContext).getValue(Constant.USERID, "0");
        long start = System.currentTimeMillis();
        int result = mRtcEngine.joinChannel(null, channelId, "Extra Optional Data", Integer.valueOf(userId));
        long end = System.currentTimeMillis();
        Log.e("ygs111", "startLive: "+(end-start));
        return result;
    }

    @Override
    public void setDefaultAudioRoutetoSpeakerphone(boolean speaker) {

    }

    @Override
    public void muteRemoteUidVideo(String username, boolean isMute) {
        if (mRtcEngine != null) {
            mRtcEngine.muteRemoteAudioStream(Integer.parseInt(username), isMute);
        }
    }

    @Override
    public void muteRemoteUidAudio(String username, boolean isMute) {
        if (mRtcEngine != null) {
            mRtcEngine.muteRemoteVideoStream(Integer.parseInt(username), isMute);
        }
    }

    @Override
    public void testInChannelTime(long time) {

    }

    @Override
    public int stopLive() {
        if (liveCallback != null) {
            liveCallback.onLeaveChanel();
        }
        return mRtcEngine.leaveChannel();


    }

    @Override
    public void enableLastTest() {
        mRtcEngine.enableLastmileTest();
    }

    @Override
    public void disableLastTest() {
        mRtcEngine.disableLastmileTest();
    }

    @Override
    public int openCamera(boolean open) {
        return mRtcEngine.enableLocalVideo(open);
    }

    @Override
    public int exchangeCamera() {
        return mRtcEngine.switchCamera();

    }

    @Override
    public int openAudioRecord(boolean open) {
        return mRtcEngine.enableLocalAudio(open);
    }

    @Override
    public int sendMessage(String msg, SendMessageCallback callback) {
        return 0;
    }


    @Override
    public SurfaceView getSurfaceview(boolean isLocalVideo) {
        SurfaceView surfaceView = RtcEngine.CreateRendererView(mContext);
        return surfaceView;
    }

    @Override
    public int setLocalVideo(SurfaceView surfaceView) {
        mRtcEngine.setupLocalVideo(new VideoCanvas(surfaceView, VideoCanvas.RENDER_MODE_FIT, 0));
        return 0;
    }

    @Override
    public int setRemoteVideo(SurfaceView surfaceView, String uid) {
        Integer intUid = Integer.valueOf(uid);
        mRtcEngine.setupRemoteVideo(new VideoCanvas(surfaceView, VideoCanvas.RENDER_MODE_FIT, intUid));
        return 0;
    }

    @Override
    public int setRemoteVideoStreamType(String uid, int mode) {
        Integer intUid = Integer.valueOf(uid);
        mRtcEngine.setRemoteVideoStreamType(intUid, mode);
        return 0;
    }


    private RtcEngine initRtcEngine() {
        long start = System.currentTimeMillis();
        if (mRtcEngine == null) {
            String appId = mContext.getString(R.string.agora_app_id);
            if (TextUtils.isEmpty(appId)) {
                throw new RuntimeException("NEED TO use your App ID, get your own ID at https://dashboard.agora.io/");
            }
            try {

                mRtcEngine = RtcEngine.create(mContext, appId, new AgoreEventHandler(liveCallback));


            } catch (Exception e) {
                throw new RuntimeException("NEED TO check rtc sdk init fatal error\n" + Log.getStackTraceString(e));
            }
            mRtcEngine.enableAudio();
            mRtcEngine.enableVideo();
            mRtcEngine.enableLocalVideo(true);
            mRtcEngine.setVideoEncoderConfiguration(new VideoEncoderConfiguration(VideoEncoderConfiguration.VD_640x360, VideoEncoderConfiguration.FRAME_RATE.FRAME_RATE_FPS_15,
                    VideoEncoderConfiguration.STANDARD_BITRATE,
                    VideoEncoderConfiguration.ORIENTATION_MODE.ORIENTATION_MODE_ADAPTIVE));

            mRtcEngine.enableDualStreamMode(true);
        }
        long end = System.currentTimeMillis();
        Log.e("ygstest123-->", "initRtcEngine: " + (end - start));
        return mRtcEngine;

    }
}
