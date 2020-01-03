package com.duobeiyun.multi_sdk_demo.bean;

import android.view.SurfaceView;

/**
 * @author : create by yanggaosheng$
 * @time : 2019-12-31$
 * @des :  视频窗口相关状态$
 **/
public class VideoBean {
    public boolean isBig;//是否是大窗口
    public boolean isLocal;//是否是本地视频
    public String uid;//对应的uid
//    public SurfaceView view;//对应的渲染view

    public VideoBean(boolean isBig, boolean isLocal, String uid) {
        this.isBig = isBig;
        this.isLocal = isLocal;
        this.uid = uid;
    }
}
