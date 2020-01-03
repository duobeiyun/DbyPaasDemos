package com.duobeiyun.multi_sdk_demo.live;

import android.content.Context;

import com.duobeiyun.multi_sdk_demo.live.impl.AgoraLiveStrategy;
import com.duobeiyun.multi_sdk_demo.live.impl.DuobeiLiveStrategy;


public class LivestrategyFactory {

    public static Livestrategy getLiveStrategyInstance(int checkLine, Context context) {
        if (checkLine == 1) {
            return new DuobeiLiveStrategy(context);
        } else {
            return new AgoraLiveStrategy(context);
        }

    }
}
