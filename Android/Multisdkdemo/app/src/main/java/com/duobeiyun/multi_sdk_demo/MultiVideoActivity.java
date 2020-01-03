package com.duobeiyun.multi_sdk_demo;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.SurfaceView;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.duobeiyun.multi_sdk_demo.bean.VideoBean;
import com.duobeiyun.multi_sdk_demo.live.LiveCallback;
import com.duobeiyun.multi_sdk_demo.live.LiveCode;
import com.duobeiyun.multi_sdk_demo.live.Livestrategy;
import com.duobeiyun.multi_sdk_demo.live.LivestrategyFactory;
import com.duobeiyun.multi_sdk_demo.utils.Constant;
import com.duobeiyun.multi_sdk_demo.utils.SharePreUtils;
import com.duobeiyun.multi_sdk_demo.utils.ViewUtils;
import com.duobeiyun.multi_sdk_demo.widget.TipsDialog;
import com.duobeiyun.paassdk.bean.DBVolumeInfo;
import com.duobeiyun.paassdk.utils.StatusCode;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.Locale;

public class MultiVideoActivity extends AppCompatActivity {
    private static final int PERMISSION_REQ_ID = 123;
    private static final String[] REQUESTED_PERMISSIONS = {Manifest.permission.RECORD_AUDIO,
            Manifest.permission.CAMERA, Manifest.permission.WRITE_EXTERNAL_STORAGE};
    private LinearLayout smallViewContainer1, smallViewContainer2;
    private RelativeLayout bigViewContaner;
    private int line = 1;//当前所在线路
    private Livestrategy mLivestrategy;
    private ProgressBar load;
    private String userId;
    boolean isLastTestOk = false; //最后一公里测试是否OK

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_multi_video);
        bigViewContaner = findViewById(R.id.rl_remote_big_container);
        smallViewContainer1 = findViewById(R.id.ll_remote_small_container);
        smallViewContainer2 = findViewById(R.id.ll_remote_small_container_bottom);
        load = findViewById(R.id.load);

    }

    @Override
    protected void onStart() {
        super.onStart();
        userId = SharePreUtils.getInstance(this.getApplicationContext()).getValue(Constant.USERID, "0");
        if (checkSelfPermission(REQUESTED_PERMISSIONS[0], PERMISSION_REQ_ID) &&
                checkSelfPermission(REQUESTED_PERMISSIONS[1], PERMISSION_REQ_ID) &&
                checkSelfPermission(REQUESTED_PERMISSIONS[2], PERMISSION_REQ_ID)) {
            initSDK();
        }
        if (mLivestrategy != null) {
            mLivestrategy.resume();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (mLivestrategy != null) {
            mLivestrategy.pause();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mLivestrategy != null) {
            mLivestrategy.destroy();
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            showTipsDialog();
            return false;
        }
        return super.onKeyDown(keyCode, event);
    }

    private void showTipsDialog() {
        final TipsDialog dialog = new TipsDialog(MultiVideoActivity.this);
        dialog.setMessage("确定要退出吗？");
        dialog.setYesOnclickListener("确定", new TipsDialog.onYesOnclickListener() {
            @Override
            public void onYesClick() {
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
        if (mLivestrategy != null) {
            load.setVisibility(View.VISIBLE);
            mLivestrategy.stopLive();
        }
    }

    private void realDestory() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {

                if (load.getVisibility() == View.VISIBLE) {
                    load.setVisibility(View.GONE);
                }
                if (MultiVideoActivity.this.isFinishing()) {
                    return;
                }
                MultiVideoActivity.this.finish();
            }
        });
    }

    //初始化Sdk相关内容
    private void initSDK() {
        int checkLine = SharePreUtils.getInstance(this.getApplicationContext()).getValue(Constant.LINE_CHECKED, 1);
        line = checkLine;
        mLivestrategy = LivestrategyFactory.getLiveStrategyInstance(checkLine, this.getApplicationContext());
        mLivestrategy.setLiveCallback(liveCallback);
        mLivestrategy.enableLastTest();//可以在进入频道前先进行测速，然后在进入频道，也可以不进行直接进入频道 mLivestrategy.startLive();；看具体业务逻辑
        if (mLivestrategy != null) {
            VideoBean bean = new VideoBean(true, true, userId);
            SurfaceView surfaceView = addLocalView(bean, false);
            bigViewContaner.addView(surfaceView);
            mLivestrategy.setLocalVideo(surfaceView);
        }
    }


    /***************************************SDK相关回调事件**************************************************************/
    private final LiveCallback liveCallback = new LiveCallback() {
        @Override
        public void onJoinChannelSuccess(String channel, final String uid) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(MultiVideoActivity.this, "进入频道成功", Toast.LENGTH_SHORT).show();
                }
            });
        }

        @Override
        public void onRejoinChannelSuccess(String channel, final String userId) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(MultiVideoActivity.this, "重连成功", Toast.LENGTH_SHORT).show();
                }
            });
        }

        @Override
        public void onLeaveChanel() {
            if (mLivestrategy != null) {
                mLivestrategy.destroy();
                mLivestrategy = null;
            }
            realDestory();
        }

        @Override
        public void onBroadCastMessage(String uid, String message) {
            //自定义的聊天消息，多贝云 PaaS SDK特有功能接口
        }

        @Override
        public void onClientkickOff() {
            //同一个id;在同一频道，有一个会受到被踢的回调，多贝云PaaS SDK特有功能接口
        }

        @Override
        public void onFirstRemoteVideoDecoded(String uid, int width, int height) {

        }

        @Override
        public void onUserOffline(String uid) {
            removeRemoteVideo(uid);
        }


        @Override
        public void onUserJoined(String uid) {
            renderRemoteVideo(uid);
        }

        @Override
        public void netDelay(int lastmileDelay) {
            //网络延时，是设备到边缘流媒体服务器的延时
        }

        @Override
        public void onLastmileQuality(final int quality) {
            if (quality == 1 || quality == 2 || quality == 3 || quality == 4 || quality == 5) {
                if (!isLastTestOk) {
                    if (mLivestrategy != null) {
                        mLivestrategy.disableLastTest();
                        mLivestrategy.startLive();
                    }
                }
                isLastTestOk = true;
            } else if (quality == 6 || quality == 7) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MultiVideoActivity.this, "当前网络有问题！code=" + quality, Toast.LENGTH_SHORT).show();
                    }
                });

            }
        }

        @Override
        public void remoteVideoStateChange(String uid, boolean state) {
            //远端视频发生改变的回调事件；可以控制视频的显示和隐藏等操作
        }


        @Override
        public void remoteMicStateChange(String uid, boolean state) {
            //远端麦克风的开关相关操作
        }

        @Override
        public void onConnectionStateChanged(final int code) {
            //todo 需要在此处处理网络连接状态的处理
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (code == LiveCode.SDK_CONNECT_FAILED) {
                        Toast.makeText(MultiVideoActivity.this, "你已断开连接，正在尝试重连", Toast.LENGTH_SHORT).show();

                    } else if (code == LiveCode.SDK_CONNECT_LOST) {
                        Toast.makeText(MultiVideoActivity.this, "SDK网络网络连接失败，退出重进试一下", Toast.LENGTH_SHORT).show();
                    }
                }
            });

        }

        @Override
        public void onStatusCode(final int code) {
            //todo 需要根据不同平台的相关错误码进行相应的干预操作，具体可以参考平台的错误码
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    String tipStr = "";
                    if (StatusCode.CHANNEL_ID_NOT_MATCH == code) {
                        tipStr = "请输入合法的频道ID ";
                    }
                    if (StatusCode.CHANNEL_UID_NOT_MATCH == code) {
                        tipStr = "输入的userId不能包含表情符号";
                    }
                    if (StatusCode.GENERATE_URL_ERROR == code) {
                        tipStr = "请检查你的网络连接";
                    }
                    load.setVisibility(View.GONE);
                    Toast.makeText(MultiVideoActivity.this, "进入频道失败(" + tipStr + " [" + code + "]" + ")",
                            Toast.LENGTH_SHORT).show();

                }
            });
        }

        @Override
        public void newWorkQuality(final String userId, final int txQuality, final int rxQuality) {
            //对应用户的网络质量
        }

        @Override
        public void onAudioVolumeIndication(final List<DBVolumeInfo> list, int totalVolume) {
            //频道内说话者声音大小的事件

        }
    };

    private void removeRemoteVideo(final String uid) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (!removeChildView(smallViewContainer1, uid)) {
                    removeChildView(smallViewContainer2, uid);
                }
            }
        });
    }

    private void renderRemoteVideo(final String uid) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (smallViewContainer1.getChildCount() > 3 && smallViewContainer2.getChildCount() > 3) {
                    return;
                }

                VideoBean bean = new VideoBean(false, false, uid);
                SurfaceView surfaceView = createRemoteRenderView(true, bean);
                addViewtoSmallContainer(surfaceView);
                mLivestrategy.setRemoteVideoStreamType(uid, 1);
                mLivestrategy.setRemoteVideo(surfaceView, uid);

            }
        });
    }

    private void processViewChange(View v, VideoBean bean) {
        if (bean.isBig) {//大窗口不可双击
            return;
        }
        bean.isBig = true;
        ViewUtils.removeViewFromParent(v);
        if (bean.isLocal) {//点击的是本地视频小窗口
            if (bigViewContaner.getChildCount() > 0) {
                View childAt = bigViewContaner.getChildAt(0);
                VideoBean tag = (VideoBean) childAt.getTag();
                tag.isBig = false;
                SurfaceView surfaceView = createRemoteRenderView(true, tag);
                addViewtoSmallContainer(surfaceView);
                mLivestrategy.setRemoteVideoStreamType(tag.uid, 1);
                mLivestrategy.setRemoteVideo(surfaceView, tag.uid);
            }
            bigViewContaner.removeAllViews();
            SurfaceView surfaceView = addLocalView(bean, false);
            bigViewContaner.addView(surfaceView);
            mLivestrategy.setLocalVideo(surfaceView);
            return;
        }
        if (bigViewContaner.getChildCount() > 0) {
            View childAt = bigViewContaner.getChildAt(0);
            VideoBean tag = (VideoBean) childAt.getTag();
            bigViewContaner.removeAllViews();
            tag.isBig = false;
            if (tag.isLocal) {
                SurfaceView surfaceView = addLocalView(tag, true);
                addViewtoSmallContainer(surfaceView);
                mLivestrategy.setLocalVideo(surfaceView);
            } else {
                SurfaceView surfaceView = createRemoteRenderView(true, tag);
                addViewtoSmallContainer(surfaceView);
                mLivestrategy.setRemoteVideoStreamType(tag.uid, 1);
                mLivestrategy.setRemoteVideo(surfaceView, tag.uid);
            }
            SurfaceView psurfaceView = createRemoteRenderView(false, bean);
            bigViewContaner.addView(psurfaceView);
            mLivestrategy.setRemoteVideoStreamType(bean.uid, 0);
            mLivestrategy.setRemoteVideo(psurfaceView, bean.uid);
        }
    }

    private boolean removeChildView(LinearLayout ll1, String uid) {
        int childCount = ll1.getChildCount();
        for (int i = 0; i < childCount; i++) {
            View childAt1 = ll1.getChildAt(i);
            if (childAt1 == null) {
                continue;
            }
            VideoBean tag = (VideoBean) childAt1.getTag();
            if (tag == null) {
                continue;
            }
            if (tag.uid.equals(uid)) {
                ll1.removeViewAt(i);
                return true;
            }
        }
        return false;
    }

    private SurfaceView createRemoteRenderView(boolean issmall, final VideoBean bean) {
        SurfaceView surfaceView = mLivestrategy.getSurfaceview(false);

        if (issmall) {
            LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(0, FrameLayout.LayoutParams.MATCH_PARENT, 1.0f);
            layoutParams.gravity = Gravity.CENTER;
            surfaceView.setLayoutParams(layoutParams);
        }
        surfaceView.setTag(bean);
        //设置双击切换
        surfaceView.setOnClickListener(new DoubleClickListener() {
            @Override
            public void onDoubleClick(View v) {
                //双击切换大视图和小视图
                processViewChange(v, bean);
            }
        });
        return surfaceView;
    }

    private SurfaceView addLocalView(final VideoBean bean, boolean small) {
        SurfaceView surfaceView = mLivestrategy.getSurfaceview(true);
        if (small) {
            LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(0, FrameLayout.LayoutParams.MATCH_PARENT, 1.0f);
            layoutParams.gravity = Gravity.CENTER;
            surfaceView.setLayoutParams(layoutParams);
        }
        surfaceView.setTag(bean);
        surfaceView.setOnClickListener(new DoubleClickListener() {
            @Override
            public void onDoubleClick(View v) {
                //双击切换大视图和小视图
                processViewChange(v, bean);
            }
        });
        return surfaceView;
    }

    private void addViewtoSmallContainer(SurfaceView surfaceView) {
        if (smallViewContainer1.getChildCount() < 3) {
            smallViewContainer1.addView(surfaceView);
        } else if (smallViewContainer2.getChildCount() < 3) {
            smallViewContainer2.addView(surfaceView);
        }
    }

    /***************************************业务无关**************************************************************/
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
                if (grantResults.length < 0) {
                    Toast.makeText(this, "请打开相应的权限再次进入", Toast.LENGTH_SHORT).show();
                    finish();
                    break;
                }
                if (grantResults[0] != PackageManager.PERMISSION_GRANTED || grantResults[1] != PackageManager.PERMISSION_GRANTED || grantResults[2] != PackageManager.PERMISSION_GRANTED) {

                    Toast.makeText(this, "请打开相应的权限再次进入", Toast.LENGTH_SHORT).show();
                    finish();
                    break;
                }
                initSDK();
                break;
            }
        }
    }

    public static void openVideoActivity(Context context) {
        Intent intent = new Intent(context, MultiVideoActivity.class);
        context.startActivity(intent);
    }
}
