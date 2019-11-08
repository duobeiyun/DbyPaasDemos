package com.duobeiyun.demo_1v1.utils;

import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;

public class ViewUtils {


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

    public static void removeParentChildView(View view) {
        if (view == null) {
            return;
        }
        ViewParent parent = view.getParent();
        if (parent != null) {
            ((FrameLayout) parent).removeView(view);
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
}
