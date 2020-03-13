package com.duobeiyun.multi_sdk_demo.live.eventhandler;

import com.duobeiyun.multi_sdk_demo.live.LiveCallback;
import com.duobeiyun.multi_sdk_demo.live.LiveCode;
import com.duobeiyun.paassdk.bean.AVideoState;
import com.duobeiyun.paassdk.bean.DBVolumeInfo;
import com.duobeiyun.paassdk.live.DbyEventHandler;
import com.duobeiyun.paassdk.utils.StatusCode;

import java.util.List;

public class DuobeiEventHandler extends DbyEventHandler {
    private LiveCallback callback;

    public DuobeiEventHandler(LiveCallback callback) {
        this.callback = callback;
    }

    @Override
    public void onUserJoined(String uid,String userName) {
        if (callback != null) {
            callback.onUserJoined(uid);
        }
    }

    @Override
    public void onUserLeave(String uid) {
        if (callback != null) {
            callback.onUserOffline(uid);
        }
    }

    @Override
    public void onJoinChannelSuccess(String channel, String userId,String userName) {
        if (callback != null) {
            callback.onJoinChannelSuccess(channel, userId);
        }
    }

    @Override
    public void onRejoinChannelSuccess(String channel, String userId,String userName) {
        if (callback != null) {
            callback.onRejoinChannelSuccess(channel, userId);
        }
    }

    @Override
    public void onLeaveChannel() {
        if (callback != null) {
            callback.onLeaveChanel();
        }
    }

    @Override
    public void onError(int errcode) {
        if (callback != null) {
            callback.onStatusCode(errcode);
        }
    }

    @Override
    public void onStatusCode(int code) {
        if (callback != null) {
//            callback.onStatusCode(code);
        }
    }


    @Override
    public void onNetWorkDelay(int delay) {
        if (callback != null) {
            callback.netDelay(delay);
        }
    }

    @Override
    public void onClientkickOff() {
        super.onClientkickOff();
        if (callback != null) {
            callback.onClientkickOff();
        }
    }

    @Override
    public void onFirstRemoteVideoDecoded(String uid, String deviceName, int width, int height) {
        if (callback != null) {
            callback.onFirstRemoteVideoDecoded(uid, width, height);
        }
    }


    @Override
    public void onConnectionStateChanged(int code) {
        super.onConnectionStateChanged(code);
        int statucode = code;
        if (code == StatusCode.CPP_APPS_CONNECT_RECONNECT) {
            statucode = LiveCode.SDK_CONNECT_FAILED;
        }
        if (code == StatusCode.CPP_APPS_CONNECT_FAIL) {
            statucode = LiveCode.SDK_CONNECT_LOST;
        }
        if (callback != null) {
            callback.onConnectionStateChanged(statucode);
        }
    }

    /**
     * @param uid   userid
     * @param state 远端视频的状态
     *              1：标识正常渲染。
     *              2: 表示对端关闭视频
     */
    @Override
    public void onRemoteVideoStateChanged(String uid, String deviceName, AVideoState state) {
        if (callback != null) {
            boolean sta = false;
            if (state == AVideoState.FIRST_FRAME || state == AVideoState.INIT) {
                sta = true;
            } else if (state == AVideoState.STOP) {
                sta = false;
            }
            callback.remoteVideoStateChange(uid, sta);
        }
    }

    @Override
    public void onRemoteMicStateChange(String uid, String deviceName, AVideoState state) {
        if (callback != null) {
            boolean sta = false;
            if (state == AVideoState.FIRST_FRAME || state == AVideoState.INIT) {
                sta = true;
            } else if (state == AVideoState.STOP) {
                sta = false;
            }
            callback.remoteMicStateChange(uid, sta);
        }
    }

    @Override
    public void onBroadCastMessage(String uid, String message) {
        if (callback != null) {
            callback.onBroadCastMessage(uid, message);
        }
    }

    @Override
    public void onNetworkQuality(String userId, int txQuality, int rxQuality) {
        super.onNetworkQuality(userId, txQuality, rxQuality);
        if (callback != null) {
            callback.newWorkQuality(userId, txQuality, rxQuality);
        }
    }

    @Override
    public void onAudioVolumeIndication(List<DBVolumeInfo> list, int totalVolume) {
        super.onAudioVolumeIndication(list, totalVolume);
        if (callback != null) {
            callback.onAudioVolumeIndication(list, totalVolume);
        }
    }

    @Override
    public void onClientRoleChanged(int oldRole, int newRole) {
        super.onClientRoleChanged(oldRole, newRole);
        //用户角色发生改变
    }

    @Override
    public void onLastmileQuality(int quality) {

        if (callback != null) {
            callback.onLastmileQuality(quality);

        }


    }


}