package com.duobeiyun.demo_1v1;

import android.app.Dialog;
import android.content.Context;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;

import java.util.Timer;
import java.util.TimerTask;

/***
 *** Created by  com.duobei.duobeiapp.common by yangge on 2017/10/23 
 *** mail:ge123.yang@gmail.com
 * 主要负责输入聊天信息的Dialog
 **/
public class MessageDialog extends Dialog {

    private final EditText mInputMessage;
    private final TextView mSendMessg;
    private final TextView mCancel;
    private InputMethodManager imm;
    private Context mContext;
    private final RelativeLayout mRootview;
    private int mLastDiff = 0;
    TextCallback callback;

    public MessageDialog(@NonNull Context context) {
        super(context, R.style.inputdialog);
        this.mContext = context;
        setContentView(R.layout.message_input_layout);
        mInputMessage = (EditText) findViewById(R.id.inputMsg);
        mSendMessg = (TextView) findViewById(R.id.sendMsg);
        mCancel = (TextView) findViewById(R.id.cancel);
        imm = (InputMethodManager) mContext.getSystemService(Context.INPUT_METHOD_SERVICE);
        mRootview = (RelativeLayout) findViewById(R.id.rl_messageroot);
        mSendMessg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (mInputMessage.getText().length() > 0) {
                    sendText("" + mInputMessage.getText());
                    imm.showSoftInput(mInputMessage, InputMethodManager.SHOW_FORCED);
                    dismiss();
                } else {
                    Toast.makeText(mContext, "输入的内容不能为空!", Toast.LENGTH_LONG).show();
                }
            }
        });
        mInputMessage.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                if (s.length() == 0) {
                    mSendMessg.setBackgroundResource(R.drawable.message_send_btn_default);
                } else {
                    mSendMessg.setBackgroundResource(R.drawable.message_send_btn_choose);

                }
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });

        mCancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dismiss();
            }
        });

    }


    public void setTextCallback(TextCallback textCallback) {
        this.callback = textCallback;
    }

    private void sendText(String s) {
        if (callback != null) {
            callback.showInputText(s);

        }
    }


    @Override
    public void show() {
        super.show();
//        activity.hidenMessageList(true);
        Timer timer = new Timer();
        timer.schedule(new TimerTask() {

            public void run() {
                InputMethodManager inputManager = (InputMethodManager) mInputMessage.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
                inputManager.showSoftInput(mInputMessage, 0);

            }

        }, 200);
    }

    @Override
    public void dismiss() {
        imm.hideSoftInputFromWindow(mInputMessage.getWindowToken(), 0);
//        activity.hidenMessageList(false);
        super.dismiss();

    }

    interface TextCallback {
        void showInputText(String msg);
    }

}
