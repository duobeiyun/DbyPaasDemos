package com.duobeiyun.demo_1v1;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.PorterDuff;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.duobeiyun.paassdk.bean.VideoState;
import com.duobeiyun.paassdk.live.DbyEngine;
import com.duobeiyun.paassdk.live.DbyEventHandler;
import com.duobeiyun.paassdk.opengles.VideoDraw;
import com.duobeiyun.paassdk.utils.StatusCode;
import com.duobeiyun.paassdk.video.CameraConfig;

import java.util.ArrayList;
import java.util.List;

public class One2OneActivity extends AppCompatActivity {
    private static final String ROOMID = "roomid";
    private static final String USERID = "userId";
    private static final int PERMISSION_REQ_ID = 123;
    private static final String[] REQUESTED_PERMISSIONS = {Manifest.permission.RECORD_AUDIO, Manifest.permission.CAMERA, Manifest.permission.WRITE_EXTERNAL_STORAGE};
    private DbyEngine mDbyEngine;
    private String roomID, userId;
    private ProgressBar load;
    private ImageView ivstopChannel;
    private ImageView ivinputMessage;
    private RecyclerView rcmessageView;
    private RelativeLayout remoteVideoContainer, localVideoContainer, tipsContainer, rlTipsTouch, rootContainer;
    private TextView tvAvUp, tvAvDown, tvAvFps, tvNetDelay, tv_protocol;
    private ChatAdapter chatAdapter;
    private List<ChatMessage> messageList = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN
                , WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_one2_one);
        load = (ProgressBar) findViewById(R.id.load);
        ivstopChannel = findViewById(R.id.iv_stop_channel);
        rootContainer = findViewById(R.id.rl_container);
        ivinputMessage = findViewById(R.id.btn_sendmsg);
        rcmessageView = findViewById(R.id.rc_view);
        remoteVideoContainer = findViewById(R.id.rl_remote_container);
        localVideoContainer = findViewById(R.id.rl_local_container);
        rlTipsTouch = findViewById(R.id.rl_touch_tips);
        tipsContainer = findViewById(R.id.rl_tips);

        tvNetDelay = findViewById(R.id.tv_avdelay);
        tvAvUp = findViewById(R.id.tv_avup);
        tvAvDown = findViewById(R.id.tv_avdown);
        tvAvFps = findViewById(R.id.tv_avfps);
        tv_protocol = findViewById(R.id.tv_protocol);
        chatAdapter = new ChatAdapter(messageList);
        rcmessageView.setLayoutManager(new LinearLayoutManager(this));
        rcmessageView.setAdapter(chatAdapter);
        Intent intent = getIntent();
        if (intent != null) {
            roomID = intent.getStringExtra(ROOMID);
            userId = intent.getStringExtra(USERID);
        }
        initClickListener();
    }

    private void initClickListener() {
        ivstopChannel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showTipsDialog();
            }
        });
        rlTipsTouch.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                processTipsShow();
                mDbyEngine.switchCamera();
            }
        });
        ivinputMessage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openMessageDialog();
            }
        });
    }


    @Override
    protected void onStart() {
        super.onStart();
        if (checkSelfPermission(REQUESTED_PERMISSIONS[0], PERMISSION_REQ_ID) &&
                checkSelfPermission(REQUESTED_PERMISSIONS[1], PERMISSION_REQ_ID) &&
                checkSelfPermission(REQUESTED_PERMISSIONS[2], PERMISSION_REQ_ID)) {
            initDbyEngine();
        }
        if (mDbyEngine != null) {
            mDbyEngine.recovery();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (mDbyEngine != null) {
            mDbyEngine.pause();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (dbyEventHandler != null) {
            dbyEventHandler = null;
        }
        if (mDbyEngine != null) {
            mDbyEngine.destory();
            mDbyEngine = null;
        }
        remoteVideoContainer.removeAllViews();

    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            showTipsDialog();
            return false;
        }
        return super.onKeyDown(keyCode, event);
    }

    private void initDbyEngine() {
        load.setVisibility(View.VISIBLE);
        initsdk();
        enableLocalCamera();

        setLocalCameraView();
        joinChannel();

    }

    private void joinChannel() {
        mDbyEngine.joinChannel(roomID, userId);
    }

    private void initsdk() {
        mDbyEngine = DbyEngine.createInstance(this.getApplicationContext(), getString(R.string.live_appkey), getString(R.string.app_id), dbyEventHandler);
    }

    private void enableLocalCamera() {
        mDbyEngine.enableLocalCamera(true);
        mDbyEngine.enableLocalAudio(true);
        mDbyEngine.enableDualStreams(true);
        mDbyEngine.setCameraConfig(new CameraConfig(CameraConfig.VD_640x480, CameraConfig.FrameFps.FrameFps_15));
    }

    private void setLocalCameraView() {
        if (localVideoContainer.getChildCount() > 0) {//暂时只处理1v1
            localVideoContainer.removeAllViews();
        }
        SurfaceView surfaceView = mDbyEngine.createRenderView(true);

        localVideoContainer.addView(surfaceView);
//        surfaceView.setAspectRatio(AspectRatio.AspectRatio_4_3);
        mDbyEngine.setLocalVideo(new VideoDraw(surfaceView, userId));

    }

    private void setRemoteVideoView(final String uid) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {

                if (remoteVideoContainer.getChildCount()>0) {
                    remoteVideoContainer.removeAllViews();
                }
                SurfaceView surfaceview = mDbyEngine.createRenderView(false);
                surfaceview.setZOrderOnTop(true);
                surfaceview.setTag(uid);

                mDbyEngine.setRemoteVideo(new VideoDraw(surfaceview, uid));
                remoteVideoContainer.addView(surfaceview);
                Log.e("ygsdemo1v1", "run:  add" );
                surfaceview.setVisibility(View.VISIBLE);
//                mDbyEngine.switchIdDualStreamMode(uid, false);
                Toast.makeText(One2OneActivity.this, uid + "上线了", Toast.LENGTH_SHORT).show();
            }
        });

    }

    private void removeRemoteVideoView(final String uid) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                View viewByTag = getViewByTag(remoteVideoContainer, uid);
                if (viewByTag != null) {
                    remoteVideoContainer.removeView(viewByTag);
                    Log.e("ygsdemo1v1", "run:  remove" );

                }
            }
        });
    }

    private void sendMessage(String s) {
        if (mDbyEngine == null) {
            return;
        }
        if (s == null) {
            return;
        }
        if ("".equals(s)) {
            return;
        }
        mDbyEngine.sendBroadcastMessage(s, null);
    }


    private DbyEventHandler dbyEventHandler = new DbyEventHandler() {


        @Override
        public void onError(final int errcode) {
            super.onError(errcode);
            One2OneActivity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    String tipStr = "";
                    if (StatusCode.CHANNEL_ID_NOT_MATCH == errcode) {
                        tipStr = "请输入合法的频道ID ";
                    }
                    if (StatusCode.CHANNEL_UID_NOT_MATCH == errcode) {
                        tipStr = "输入的userId不能包含表情符号";
                    }
                    Toast.makeText(One2OneActivity.this, "进入频道失败(" + tipStr + errcode + ")", Toast.LENGTH_LONG).show();
                    load.setVisibility(View.GONE);
                    exit();
                }
            });
        }

        @Override
        public void onJoinChannelSuccess(String channel, String useId) {
            One2OneActivity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    //加入成功
                    if (load.getVisibility() == View.VISIBLE) {
                        load.setVisibility(View.GONE);
                    }
                }
            });
        }

        @Override
        public void onStatusCode(int code) {
            super.onStatusCode(code);
        }

        @Override
        public void onLeaveChannel() {

            //销毁activity
            One2OneActivity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (!One2OneActivity.this.isFinishing()) {
                        One2OneActivity.this.finish();
                    }
                }
            });
        }

        @Override
        public void onUserJoined(String uid) {
            super.onUserJoined(uid);
            Log.e("ygsdemo1v1", "onUserJoined: "+uid);
            //其他人进入房间
            setRemoteVideoView(uid);
        }

        @Override
        public void onUserLeave(final String uid) {
            super.onUserLeave(uid);
            Log.e("ygsdemo1v1", "onUserLeave: "+uid);
            //其他人离开
            removeRemoteVideoView(uid);
        }

        @Override
        public void onClientkickOff() {
            super.onClientkickOff();
            One2OneActivity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(One2OneActivity.this, "你的Id在另一台设备上登录", Toast.LENGTH_LONG).show();
                    exit();
                }
            });
        }


        @Override
        public void onBroadCastMessage(final String uid, final String message) {

            //仅仅测试一下
            messageList.add(new ChatMessage(uid, message));
            //收到广播消息
            One2OneActivity.this.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    chatAdapter.notifyDataSetChanged();
                    rcmessageView.smoothScrollToPosition(chatAdapter.getItemCount() - 1);
                }
            });
        }

    };

    private void openMessageDialog() {

        MessageDialog mInputMsgDialog = new MessageDialog(this);
        WindowManager windowManager = getWindowManager();
        Display display = windowManager.getDefaultDisplay();
        WindowManager.LayoutParams lp = mInputMsgDialog.getWindow().getAttributes();

        lp.width = display.getWidth() - 10; //设置宽度
        lp.gravity = Gravity.BOTTOM;
        mInputMsgDialog.getWindow().setAttributes(lp);
        mInputMsgDialog.setCancelable(true);
        mInputMsgDialog.setCanceledOnTouchOutside(true);
        mInputMsgDialog.show();
        mInputMsgDialog.setTextCallback(new MessageDialog.TextCallback() {
            @Override
            public void showInputText(String msg) {
                sendMessage(msg);
            }
        });

    }

    private long firstTime = 0;
    private boolean showTips = false;
    private int clickCount = 0;

    private void processTipsShow() {
        long secondTime = System.currentTimeMillis();
        if (secondTime - firstTime < 1000) {
            clickCount++;
            if (clickCount >= 2) {
                showTips = !showTips;
                if (showTips) {
                    tipsContainer.setVisibility(View.VISIBLE);
                } else {
                    tipsContainer.setVisibility(View.GONE);
                }
                clickCount = 0;
            }
        } else {
            firstTime = System.currentTimeMillis();
            clickCount = 0;
        }
    }

    private void showTipsDialog() {
        final TipsDialog dialog = new TipsDialog(One2OneActivity.this);
        dialog.setMessage("确定要退出吗？");
        dialog.setYesOnclickListener("确定", new TipsDialog.onYesOnclickListener() {
            @Override
            public void onYesClick() {
                changeBackColor(ivstopChannel);
                exit();
                if (dialog.isShowing()) {
                    dialog.dismiss();
                }
            }
        });
        dialog.setNoOnclickListener("取消", new TipsDialog.onNoOnclickListener() {
            @Override
            public void onNoClick() {
                if (dialog.isShowing()) {
                    dialog.dismiss();
                }
            }
        });
        dialog.show();
    }


    private void exit() {
        if (mDbyEngine != null) {
            load.setVisibility(View.VISIBLE);
            mDbyEngine.leaveChannel();
        }
    }

    private void changeBackColor(ImageView iv) {
        if (iv.isSelected()) {
            iv.setSelected(false);
            iv.clearColorFilter();
        } else {
            iv.setSelected(true);
            iv.setColorFilter(getResources().getColor(R.color.colorPrimary), PorterDuff.Mode.MULTIPLY);
        }
    }

    public boolean checkSelfPermission(String permission, int requestCode) {
        if (ContextCompat.checkSelfPermission(this,
                permission)
                != PackageManager.PERMISSION_GRANTED) {

            ActivityCompat.requestPermissions(this,
                    REQUESTED_PERMISSIONS,
                    requestCode);
            return false;
        }
        return true;
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           @NonNull String permissions[], @NonNull int[] grantResults) {

        switch (requestCode) {
            case PERMISSION_REQ_ID: {
                for (int i = 0; i < grantResults.length; i++) {
                    if (grantResults[i] != PackageManager.PERMISSION_GRANTED) {

                        Toast.makeText(this, "请打开相应的权限再次进入", Toast.LENGTH_SHORT).show();
                        finish();
                        break;
                    }

                }
                initDbyEngine();
                break;
            }
        }
    }

    public static void openOne2OneActivity(Context context, String roomId, String userId) {
        Intent intent = new Intent(context, One2OneActivity.class);
        intent.putExtra(ROOMID, roomId);
        intent.putExtra(USERID, userId);
        context.startActivity(intent);
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
