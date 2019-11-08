package com.duobeiyun.demo_1v1;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.Settings;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    EditText et_channel, et_userId;
    Button btn_goLive,btn_go_playback;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        final String chanelId = SharePreUtils.getInstance(this.getApplicationContext()).getValue(Constant.CHANNELID, "");
        final String userId = SharePreUtils.getInstance(this.getApplicationContext()).getValue(Constant.USERID, "");
        et_channel = findViewById(R.id.et_channel_id);
        et_userId = findViewById(R.id.et_user_id);
        btn_goLive = findViewById(R.id.btn_go_live);
        btn_go_playback = findViewById(R.id.btn_go_playback);
        btn_goLive.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                goLive();
            }
        });
        btn_go_playback.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                goPlayback();
            }
        });
        et_channel.setText(chanelId);
        et_userId.setText(userId);
    }

    private void goLive() {
        String chanelId = et_channel.getText().toString().trim();
        String userId = et_userId.getText().toString().trim();
        if (chanelId == null || "".equals(chanelId)) {
            return;
        }
        if (userId == null || "".equals(userId)) {
            return;
        }
        SharePreUtils.getInstance(MainActivity.this.getApplicationContext()).setValue(Constant.CHANNELID, chanelId);
        SharePreUtils.getInstance(MainActivity.this.getApplicationContext()).setValue(Constant.USERID, userId);

        One2OneActivity.openOne2OneActivity(MainActivity.this, chanelId, userId);
    }
    private void goPlayback() {
        GeneratePlaybackActivity.openGeneratePlaybackActivity(this);
    }

}
