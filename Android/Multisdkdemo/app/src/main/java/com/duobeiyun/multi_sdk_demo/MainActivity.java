package com.duobeiyun.multi_sdk_demo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.RadioButton;

import com.duobeiyun.multi_sdk_demo.utils.Constant;
import com.duobeiyun.multi_sdk_demo.utils.SharePreUtils;

public class MainActivity extends AppCompatActivity {

    EditText et_channel, et_userId;
    Button btn_goLive;
    private RadioButton rbOne, rbTwo;
    private CheckBox cbProfile;
    private CheckBox cbAnchor;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        final String chanelId = SharePreUtils.getInstance(this.getApplicationContext()).getValue(Constant.CHANNELID, "");
        final String userId = SharePreUtils.getInstance(this.getApplicationContext()).getValue(Constant.USERID, "");
        int checkLine = SharePreUtils.getInstance(this.getApplicationContext()).getValue(Constant.LINE_CHECKED, 1);
        boolean isLive = SharePreUtils.getInstance(this.getApplicationContext()).getValue(Constant.IS_LIVE, false);
        boolean isAnchor = SharePreUtils.getInstance(this.getApplicationContext()).getValue(Constant.IS_ANCHOR, false);
        et_channel = findViewById(R.id.et_channel_id);
        et_userId = findViewById(R.id.et_user_id);
        btn_goLive = findViewById(R.id.btn_go_live);
        rbOne = findViewById(R.id.rb_line_one);
        rbTwo = findViewById(R.id.rb_line_two);
        cbProfile = findViewById(R.id.cb_live_Profile);
        cbAnchor = findViewById(R.id.cb_live_Profile);
        cbProfile.setChecked(isLive);
        cbAnchor.setChecked(isAnchor);
        btn_goLive.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                goLive();
            }
        });
        if (checkLine == 1) {
            rbOne.setChecked(true);
        } else {
            rbTwo.setChecked(true);
        }
        et_channel.setText(chanelId);
        et_userId.setText(userId);
    }

    private void goLive() {
        String chanelId = et_channel.getText().toString().trim();
        String userId = et_userId.getText().toString().trim();
        if ("".equals(chanelId)) {
            return;
        }
        if ("".equals(userId)) {
            return;
        }
        boolean isLive = cbProfile.isChecked();
        boolean isAnchor = cbAnchor.isChecked();
        SharePreUtils.getInstance(MainActivity.this.getApplicationContext()).setValue(Constant.LINE_CHECKED, rbOne.isChecked() ? 1 : 2);
        SharePreUtils.getInstance(MainActivity.this.getApplicationContext()).setValue(Constant.CHANNELID, chanelId);
        SharePreUtils.getInstance(MainActivity.this.getApplicationContext()).setValue(Constant.USERID, userId);
        SharePreUtils.getInstance(MainActivity.this.getApplicationContext()).setValue(Constant.IS_LIVE, isLive);
        SharePreUtils.getInstance(MainActivity.this.getApplicationContext()).setValue(Constant.IS_ANCHOR, isAnchor);

        MultiVideoActivity.openVideoActivity(MainActivity.this);
    }
}
