package com.duobeiyun.multi_sdk_demo.utils;

import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

public class ViewUtils {


    public static void removeViewFromParent(View view) {
        if (view.getParent() instanceof LinearLayout) {
            removeLinearLayoutParentChildView(view);
        }
        if (view.getParent() instanceof RelativeLayout) {
            removeRelativeLayoutParentChildView(view);
        }
    }


    public static void removeLinearLayoutParentChildView(View view) {
        if (view == null) {
            return;
        }
        ViewParent parent = view.getParent();
        if (parent != null) {
            ((LinearLayout) parent).removeView(view);
        }
    }

    public static void removeRelativeLayoutParentChildView(View view) {
        if (view == null) {
            return;
        }
        ViewParent parent = view.getParent();
        if (parent != null) {
            ((RelativeLayout) parent).removeView(view);
        }
    }

    public static View getViewByTag(ViewGroup parentView, String tag) {
        if (parentView == null) {
            return null;
        }
        int childCount = parentView.getChildCount();
        for (int i = 0; i < childCount; i++) {
            View childView = parentView.getChildAt(i);
            if (tag.equals((String) childView.getTag())) {
                return childView;
            }
        }
        return null;

    }

}
