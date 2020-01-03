package com.duobeiyun.multi_sdk_demo.live;

import com.duobeiyun.paassdk.bean.DBVolumeInfo;
import com.duobeiyun.paassdk.bean.stats.RemoteVideoStats;

import java.util.List;

/**
 * 正对strategy的相关处理的回调；ui正对性处理
 */
public interface LiveCallback {


    void onJoinChannelSuccess(String channel, String uid);

    void onUserOffline(String uid);

    void onUserJoined(String uid);

    void netDelay(int lastmileDelay);

    void remoteVideoStateChange(String uid, boolean state);

    void onStatusCode(int code);

    void onLeaveChanel();

    void onBroadCastMessage(String uid, String message);

    void onClientkickOff();

    void onFirstRemoteVideoDecoded(String uid, int width, int height);

    void onConnectionStateChanged(int code);

    void remoteMicStateChange(String uid, boolean state);

    void onRejoinChannelSuccess(String channel, String userId);

    void newWorkQuality(String userId, int txQuality, int rxQuality);

    void onAudioVolumeIndication(List<DBVolumeInfo> list, int totalVolume);

    void onLastmileQuality(int quality);

}
