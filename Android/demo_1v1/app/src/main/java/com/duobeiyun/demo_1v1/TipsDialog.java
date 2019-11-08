package com.duobeiyun.demo_1v1;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class TipsDialog extends Dialog {

    private Button yes;
    private Button no;
    private TextView titleTv;
    private TextView messageTv;
    private String titleStr;
    private String messageStr;

    private String yesStr, noStr;

    private onNoOnclickListener noOnclickListener;
    private onYesOnclickListener yesOnclickListener;

    public void setNoOnclickListener(String str, onNoOnclickListener onNoOnclickListener) {
        if (str != null) {
            noStr = str;
        }
        this.noOnclickListener = onNoOnclickListener;
    }


    public void setYesOnclickListener(String str, onYesOnclickListener onYesOnclickListener) {
        if (str != null) {
            yesStr = str;
        }
        this.yesOnclickListener = onYesOnclickListener;
    }

    public TipsDialog(Context context) {
        super(context, R.style.DialogStyle);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.tips_dialog_layout);
        //按空白处不能取消动画
        setCanceledOnTouchOutside(false);

        initView();
        initData();
        initEvent();

    }


    private void initEvent() {
        yes.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (yesOnclickListener != null) {
                    yesOnclickListener.onYesClick();
                }
            }
        });
        no.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (noOnclickListener != null) {
                    noOnclickListener.onNoClick();
                }
            }
        });
    }


    private void initData() {
        if (titleStr != null) {
            titleTv.setText(titleStr);
        }
        if (messageStr != null) {
            messageTv.setText(messageStr);
        }
        if (yesStr != null) {
            yes.setText(yesStr);
        }
        if (noStr != null) {
            no.setText(noStr);
        }
    }


    private void initView() {
        yes = (Button) findViewById(R.id.yes);
        no = (Button) findViewById(R.id.no);
        titleTv = (TextView) findViewById(R.id.title);
        messageTv = (TextView) findViewById(R.id.message);
    }


    public void setTitle(String title) {
        titleStr = title;
    }


    public void setMessage(String message) {
        messageStr = message;
    }

    public void setCancelShow(boolean isShow) {
        if (no==null) {
            return;
        }
        if (isShow) {
            no.setVisibility(View.VISIBLE);
        } else {
            no.setVisibility(View.GONE);
        }
    }

    public interface onYesOnclickListener {
        void onYesClick();
    }

    public interface onNoOnclickListener {
        void onNoClick();
    }
}
