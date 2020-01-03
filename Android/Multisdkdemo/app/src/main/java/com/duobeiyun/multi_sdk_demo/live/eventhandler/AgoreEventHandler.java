package com.duobeiyun.multi_sdk_demo.live.eventhandler;

import android.util.Log;


import com.duobeiyun.multi_sdk_demo.live.LiveCallback;
import com.duobeiyun.multi_sdk_demo.live.LiveCode;

import io.agora.rtc.IRtcEngineEventHandler;

public class AgoreEventHandler extends IRtcEngineEventHandler {

    private LiveCallback callback;

    public AgoreEventHandler(LiveCallback callback) {
        this.callback = callback;
    }

    @Override
    public void onUserJoined(int uid, int elapsed) {
        if (callback != null) {
            callback.onUserJoined(uid + "");
        }
    }

    @Override
    public void onFirstRemoteVideoDecoded(int uid, int width, int height, int elapsed) {
        Log.e("ygstest", "onFirstRemoteVideoDecoded: " + uid);
        if (callback != null) {
            callback.onFirstRemoteVideoDecoded(uid + "", width, height);
        }
    }


    @Override
    public void onUserOffline(int uid, int reason) {
        if (callback != null) {
            callback.onUserOffline(uid + "");
        }
    }

    @Override
    public void onConnectionStateChanged(int state, int reason) {
        super.onConnectionStateChanged(state, reason);
        if (state == 5) {
            if (callback != null) {
                callback.onConnectionStateChanged(LiveCode.SDK_CONNECT_FAILED);
            }
        }

    }

    @Override
    public void onJoinChannelSuccess(String channel, int uid, int elapsed) {
        if (callback != null) {
            callback.onJoinChannelSuccess(channel, uid + "");
        }
    }

    @Override
    public void onRejoinChannelSuccess(String channel, int uid, int elapsed) {
        super.onRejoinChannelSuccess(channel, uid, elapsed);
        if (callback != null) {
            callback.onRejoinChannelSuccess(channel, uid + "");
        }
    }

    @Override
    public void onRtcStats(RtcStats stats) {
        if (callback != null) {
            callback.netDelay(stats.lastmileDelay);
        }
    }

    @Override
    public void onUserEnableLocalVideo(int uid, boolean enabled) {
        Log.e("ygstest", "onUserEnableLocalVideo: " + uid);
        if (callback != null) {
//            callback.remoteVideoStateChange(uid + "", enabled);
        }
    }

    @Override
    public void onConnectionLost() {
        super.onConnectionLost();
        if (callback != null) {
            callback.onConnectionStateChanged(LiveCode.SDK_CONNECT_LOST);
        }
    }


    @Override
    public void onLastmileQuality(int quality) {

        if (callback != null) {

            if (quality == 0) {
                quality = 1;
            }
            callback.onLastmileQuality(quality);
        }
    }

    @Override
    public void onRemoteVideoStateChanged(int uid, int state, int reason, int elapsed) {
        if (callback != null) {
            boolean sta = false;
            if (state == 1) {
                sta = true;
            } else if (state == 0) {
                sta = false;
            }
            callback.remoteVideoStateChange(uid + "", sta);
        }
    }

    @Override
    public void onRemoteAudioStateChanged(int uid, int state, int reason, int elapsed) {
        if (callback != null) {
            boolean sta = false;
            if (state == 1) {
                sta = true;
            } else if (state == 0) {
                sta = false;
            }
            callback.remoteMicStateChange(uid + "", sta);
        }
    }

    @Override
    public void onError(int err) {
        super.onError(err);
        Log.e("gystest", "onError: " + err);
        if (callback != null) {
            callback.onStatusCode(err);
        }
    }

    @Override
    public void onWarning(int warn) {
        super.onWarning(warn);
        Log.e("gystest", "onWarning: " + warn);
    }
}
