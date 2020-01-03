package com.duobeiyun.multi_sdk_demo;

import android.view.View;

/**
 * @author : create by yanggaosheng$
 * @time : 2019-12-31$
 * @des :  双击点击监听$
 **/
public abstract class DoubleClickListener implements View.OnClickListener {
    private static final long DOUBLE_TIME = 1000;
    private static long lastClickTime = 0;

    @Override
    public void onClick(View v) {
        long currentTimeMillis = System.currentTimeMillis();
        if (currentTimeMillis - lastClickTime < DOUBLE_TIME) {
            onDoubleClick(v);
        }
        lastClickTime = currentTimeMillis;
    }

    public abstract void onDoubleClick(View v);
}
