package com.duobeiyun.multi_sdk_demo.live.impl;

import android.content.Context;
import android.util.Log;
import android.view.SurfaceView;

import com.duobeiyun.multi_sdk_demo.R;
import com.duobeiyun.multi_sdk_demo.live.LiveCallback;
import com.duobeiyun.multi_sdk_demo.live.Livestrategy;
import com.duobeiyun.multi_sdk_demo.live.eventhandler.DuobeiEventHandler;
import com.duobeiyun.multi_sdk_demo.utils.Constant;
import com.duobeiyun.multi_sdk_demo.utils.SharePreUtils;
import com.duobeiyun.paassdk.live.DbyEngine;
import com.duobeiyun.paassdk.live.DbyEventHandler;
import com.duobeiyun.paassdk.opengles.GLFrameSurfaceView;
import com.duobeiyun.paassdk.opengles.VideoDraw;
import com.duobeiyun.paassdk.utils.AspectRatio;
import com.duobeiyun.paassdk.video.CameraConfig;

public class DuobeiLiveStrategy implements Livestrategy {
    private DbyEngine mDbyEngine;
    private Context mContext;
    private LiveCallback liveCallback;
    private String userId;

    public DuobeiLiveStrategy(Context mContext) {
        this.mContext = mContext;
    }

    @Override
    public void setLiveCallback(LiveCallback callback) {
        this.liveCallback = callback;
        initDbyEngine();
    }

    @Override
    public void resume() {
        if (mDbyEngine != null) {
            mDbyEngine.recovery();
        }
    }

    @Override
    public void pause() {
        if (mDbyEngine != null) {
            mDbyEngine.pause();
        }
    }

    @Override
    public void destroy() {
        if (mDbyEngine != null) {
            mDbyEngine.destroy();
        }
    }

    @Override
    public int startLive() {
        joinChannel();
        return 0;
    }

    @Override
    public int stopLive() {
        if (mDbyEngine != null) {
            mDbyEngine.leaveChannel();
        }
        return 0;
    }

    @Override
    public int openCamera(boolean open) {
        return mDbyEngine.enableLocalCamera(null, open);
    }

    @Override
    public int exchangeCamera() {
        mDbyEngine.switchCamera();
        return 0;
    }

    @Override
    public int openAudioRecord(boolean open) {
        mDbyEngine.enableLocalAudio(open);
        return 0;
    }

    @Override
    public int sendMessage(String msg, final SendMessageCallback callback) {
        mDbyEngine.sendBroadcastMessage(msg, new DbyEventHandler.BroadCastMessageHandler() {
            @Override
            public void onSendBroadCastMessageResult(int code) {
                Log.e("sendMessage", "onSendBroadCastMessageResult: " + code);
                if (callback != null) {
                    callback.sendCallback(code);
                }
            }
        });
        return 0;
    }

    @Override
    public SurfaceView getSurfaceview(boolean isLocalVideo) {
        if (!isLocalVideo) {
            GLFrameSurfaceView surfaceView = (GLFrameSurfaceView) mDbyEngine.createRenderView(isLocalVideo);
            return surfaceView;
        }
        SurfaceView surfaceView = mDbyEngine.createRenderView(isLocalVideo);
        return surfaceView;
    }

    @Override
    public int setLocalVideo(SurfaceView surfaceView) {
        mDbyEngine.setLocalVideo(new VideoDraw(surfaceView, userId));
        return 0;
    }

    @Override
    public int setRemoteVideo(SurfaceView surfaceView, String uid) {
        mDbyEngine.setRemoteVideo("", new VideoDraw(surfaceView, uid));
        return 0;
    }

    @Override
    public int setRemoteVideoStreamType(String uid, int mode) {
        mDbyEngine.setRemoteVideoStreamType(uid, mode);
        return 0;
    }

    @Override
    public void setDefaultAudioRoutetoSpeakerphone(boolean speaker) {
        mDbyEngine.setDefaultAudioRoutetoSpeakerphone(speaker);
    }

    @Override
    public void muteRemoteUidVideo(String username, boolean isMute) {
        mDbyEngine.muteRemoteAudioStream(username, "", isMute);
    }

    @Override
    public void muteRemoteUidAudio(String username, boolean isMute) {
        mDbyEngine.muteRemoteVideoStream(username, "", isMute);
    }

    @Override
    public void testInChannelTime(long time) {
        mDbyEngine.testInChannelTime(time);
    }

    @Override
    public void enableLastTest() {
        mDbyEngine.enableLastmileTest();
    }

    @Override
    public void disableLastTest() {
        mDbyEngine.disableLastmileTest();
    }

    private void initDbyEngine() {
        initsdk();
        enableLocalCamera();
    }

    private void initsdk() {
        mDbyEngine = DbyEngine.createInstance(mContext, mContext.getString(R.string.live_appkey),
                mContext.getString(R.string.live_appid), new DuobeiEventHandler(liveCallback));
        boolean isDuaStream = SharePreUtils.getInstance(mContext).getValue(Constant.DUA_STREAM, true);
        Log.e("DuobeiLiveStrategy", "initsdk: " + isDuaStream);
        mDbyEngine.enableDualStreams(isDuaStream);//需要在joinchannel前调用
    }

    private void enableLocalCamera() {
        mDbyEngine.enableLocalAudio("audio_out", true);
        mDbyEngine.enableLocalAudio(true);
        mDbyEngine.enableLocalCamera(null, true);
        mDbyEngine.setCameraConfig(new CameraConfig(CameraConfig.VD_640x360, CameraConfig.FrameFps.FrameFps_15));
    }


    private void joinChannel() {
        Log.e("ygstestttt", "joinChannel_qian");
        String channelId = SharePreUtils.getInstance(mContext).getValue(Constant.CHANNELID, "123");
        userId = SharePreUtils.getInstance(mContext).getValue(Constant.USERID, "0");
        mDbyEngine.joinChannel(channelId, userId);
        mDbyEngine.enableAudioVolumeIndication(-1, true);
    }
}
