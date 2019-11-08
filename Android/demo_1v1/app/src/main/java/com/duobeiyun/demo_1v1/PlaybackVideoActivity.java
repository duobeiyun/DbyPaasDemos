package com.duobeiyun.demo_1v1;

import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.duobeiyun.demo_1v1.adapter.VideoListAdapter;
import com.duobeiyun.demo_1v1.adapter.VideoViewChangeListener;
import com.duobeiyun.demo_1v1.bean.CustomEventBean;
import com.duobeiyun.demo_1v1.bean.VideoBean;
import com.duobeiyun.demo_1v1.utils.TimeUtils;
import com.duobeiyun.paassdk.bean.VideoState;
import com.duobeiyun.paassdk.callback.ResultCallback;
import com.duobeiyun.paassdk.opengles.VideoDraw;
import com.duobeiyun.paassdk.playback.DbyPlaybackEngine;
import com.duobeiyun.paassdk.playback.DbyPlaybackEventHandler;
import com.duobeiyun.paassdk.utils.JsonUtils;
import com.duobeiyun.paassdk.utils.StatusCode;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class PlaybackVideoActivity extends AppCompatActivity {

    private FrameLayout remotBigContainer, remoteVideoLoadContainer;
    private RecyclerView smallViewRecycleView, mUserOnlineView;
    private DbyPlaybackEngine dbyPlaybackEngine;
    private String recordId = "a1dfc2e8ac6a4a45b626a12f19230097";//"a1dfc2e8ac6a4a45b626a12f19230097   52ef0da88ae4421190bfeaa3ad3570c4";
    private ImageButton controBtn;
    private SeekBar seekBar;
    private TextView textCurrent;
    private TextView textTotalTime;
    private ProgressBar load;
    private HashMap<String, VideoBean> videoBeanHashMap = new HashMap<>();
    private VideoListAdapter videoListAdapter;
    private static final int PROGRESSMAX = 500;
    boolean isPlay = false;
    private TextView tvSpeed;
    private float speed = 1.0f;
    private static final int UPDATE_EVENT_TIME = 1000;
    private List<CustomEventBean> customEventList;
    private long mCourseStartTime;
    private volatile ArrayList<ChatMessage> messageListAD = new ArrayList<>();
    private volatile ArrayList<ChatMessage> messageList = new ArrayList<>();
    private long currentTime;
    private ChatAdapter chatAdapter;
    private Thread thread;
    private TextView tvBigUserName;
    private boolean isStart = false;
    private int lastSeekPosition = 0;
    private ImageView cameraTipsImg;
    private TextView cameraTipText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN
                , WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_playback_video);
        remotBigContainer = findViewById(R.id.remote_video_container);
        smallViewRecycleView = findViewById(R.id.rv_list);
        controBtn = findViewById(R.id.portrait_control);
        seekBar = findViewById(R.id.portrait_seekBar);
        textCurrent = findViewById(R.id.portrait_current_time);
        textTotalTime = findViewById(R.id.portrait_total_time);
        load = findViewById(R.id.load);
        tvBigUserName = findViewById(R.id.tv_big_user_id);
        tvSpeed = findViewById(R.id.tv_playback_speed);
        videoListAdapter = new VideoListAdapter(videoBeanHashMap);
        LinearLayoutManager linearLayoutManager = new LinearLayoutManager(this);
        linearLayoutManager.setOrientation(LinearLayoutManager.HORIZONTAL);
        smallViewRecycleView.setLayoutManager(linearLayoutManager);
        smallViewRecycleView.setAdapter(videoListAdapter);
        mUserOnlineView = findViewById(R.id.rv_user_online);
        remoteVideoLoadContainer = findViewById(R.id.remote_video_load_container);
        chatAdapter = new ChatAdapter(messageListAD);
        mUserOnlineView.setLayoutManager(new LinearLayoutManager(this));
        mUserOnlineView.setAdapter(chatAdapter);
        remoteVideoLoadContainer.addView(getLoad());
        remoteVideoLoadContainer.setVisibility(View.GONE);
        cameraTipsImg = findViewById(R.id.iv_camera_close_tip);
        cameraTipText = findViewById(R.id.tv_camera_tips);

        recordId = SharePreUtils.getInstance(this.getApplicationContext()).getValue(Constant.CHANNEL_RECORD_ID, recordId);
        initDbyEngine();
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {


            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (!isPlay || !isStart) {
                    return;
                }
                long currentTime = seekBar.getProgress() * mTotalTime / 500;
                String timeStr = TimeUtils.long2TimeString(currentTime);
                if (textCurrent != null) {
                    textCurrent.setText(timeStr);
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                lastSeekPosition = seekBar.getProgress();
                if (!isPlay || !isStart) {
                    return;
                }
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                if (!isStart) {
                    seekBar.setProgress(lastSeekPosition);
                    return;
                }
                int progress = seekBar.getProgress();
                if (progress == 500) {
                    progress = 499;//暂时处理拖动进度条到500导致底层传递到上层的值为负数
                }
                currentTime = progress * mTotalTime / 500;
                load.setVisibility(View.VISIBLE);
                dbyPlaybackEngine.seek(currentTime, new ResultCallback() {
                    @Override
                    public void callResult(final int i) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                if (i != 0) {
                                    Toast.makeText(PlaybackVideoActivity.this, "seek失败", Toast.LENGTH_SHORT).show();
                                }
                                isSeeking = false;
                                global_time = currentTime;
                                if (!isPlay) {
                                    if (dbyPlaybackEngine != null) {
                                        dbyPlaybackEngine.play();
                                    }
                                    isPlay = !isPlay;
                                }
                                controBtn.setBackground(getResources().getDrawable(R.drawable.playback_play));
                                load.setVisibility(View.GONE);
                            }
                        });
                    }
                });
            }
        });
        controBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!isStart) {
                    initDbyEngine();
                    return;
                }
                if (isPlay) {
                    dbyPlaybackEngine.pause();
                } else {
                    dbyPlaybackEngine.play();
                }
                isPlay = !isPlay;
                changeControlViewBackground();
            }
        });
        tvSpeed.setClickable(true);
        tvSpeed.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                float oldSpeed = speed;
                if (speed < 2.0f) {
                    speed = speed + 0.5f;
                } else {
                    speed = 1.0f;
                }
                float result = dbyPlaybackEngine.setSpeed(speed);
                if (result < 0) {
                    speed = oldSpeed;
                }
                tvSpeed.setText(result + "倍速");
            }
        });

        videoListAdapter.setVideoViewChangeListener(new VideoViewChangeListener() {
            @Override
            public void onItemChange(Object item) {
                //大小屏幕切换
                VideoBean bean = (VideoBean) item;
                changeScreen(bean);

            }

            @Override
            public void surfaceClick(VideoBean bean) {

            }
        });


    }

    @Override
    protected void onResume() {
        super.onResume();
        if (dbyPlaybackEngine != null) {
            dbyPlaybackEngine.onResumeApi();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (dbyPlaybackEngine != null) {
            dbyPlaybackEngine.onPauseApi();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (dbyPlaybackEngine != null) {
            dbyPlaybackEngine.destory();
        }
    }

    private void initDbyEngine() {
        load.setVisibility(View.VISIBLE);
        videoListAdapter.setDataSource(videoBeanHashMap);

        dbyPlaybackEngine = DbyPlaybackEngine.createInstance(this.getApplicationContext(), getString(R.string.live_appkey), getString(R.string.app_id), dbyPlaybackEventHandler);
        //进入回放
        //https://playback2.duobeiyun.net/d6a95bdbe5c5467bbdef1637d64997f5/playback.dat
        dbyPlaybackEngine.joinChannel(recordId);
    }

    private void stopDbyPlayEngine() {
        if (dbyPlaybackEngine != null) {
            dbyPlaybackEngine.leaveChannel();
        }
    }

    private long mTotalTime = 1;
    private DbyPlaybackEventHandler dbyPlaybackEventHandler = new DbyPlaybackEventHandler() {

        /**
         *说明：该handler中的方法的调用都是在子线程中执行，
         */


        @Override
        public void durationTimeCallBack(long currentTime) {
            //500标识progress的最大值，可以自行设置
            PlaybackVideoActivity.this.currentTime = currentTime;
            final int progress = (int) (currentTime * PROGRESSMAX / mTotalTime);
            final String timeStr = TimeUtils.long2TimeString(currentTime);
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (textCurrent == null) {
                        return;
                    }
                    textCurrent.setText(timeStr);
                    if (seekBar == null) {
                        return;
                    }
                    seekBar.setProgress(progress);
                }
            });
        }

        @Override
        public void totalTimeCallBack(long totalTime) {
            mTotalTime = totalTime;
            final String timeStr = TimeUtils.long2TimeString(totalTime);
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    textTotalTime.setText(timeStr);
                }
            });
        }



        @Override
        public void customEventsCallback(long courseStartTime, String jsonStr) {
            mCourseStartTime = courseStartTime;
            customEventList = JsonUtils.jsonToList(jsonStr, CustomEventBean.class);
        }

        @Override
        public void playEndCallBack() {
            isPlay = false;
            isStart = false;
            global_time = 0;
            if (thread != null) {
                thread.interrupt();
            }
            //播放完成回调
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    //重置界面相关资源
                    textCurrent.setText(TimeUtils.long2TimeString(0));
                    textTotalTime.setText(TimeUtils.long2TimeString(0));
                    seekBar.setProgress(0);
                    changeControlViewBackground();
                    if (videoBeanHashMap != null) {
                        if (!videoBeanHashMap.isEmpty()) {
                            videoBeanHashMap.clear();
                        }
                    }
                    if (videoListAdapter != null) {
                        videoListAdapter.setDataSource(videoBeanHashMap);
                    }
                    tvBigUserName.setText("");
                    remotBigContainer.removeAllViews();

                }
            });
        }

        @Override
        public void onError(final int errcode) {
            super.onError(errcode);
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    String errMessage = "";
                    if (errcode == StatusCode.Playback.RECORD_DEFAULT) {
                        errMessage = "回放还未开始转换";
                    }
                    if (errcode == StatusCode.Playback.RECORD_PROCESS) {
                        errMessage = "回放正在转换中";
                    }
                    if (errcode == StatusCode.Playback.RECORD_FAILED) {
                        errMessage = "回放转换失败";
                    }
                    if (errcode == StatusCode.Playback.RECORD_EMPTY) {
                        errMessage = "这段回放是空录像";
                    }
                    Toast.makeText(PlaybackVideoActivity.this, "进入回放失败" + errMessage + "[" + errcode + "]", Toast.LENGTH_SHORT).show();
                    load.setVisibility(View.GONE);
                }
            });
        }

        @Override
        public void onStatusCode(final int code) {
            super.onStatusCode(code);
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    String errMessage = "";
                    if (code == StatusCode.Playback.PB_READ_TIMEOUT) {
                        //文件读取超时
                        errMessage = "回放读取文件失败";
                        Toast.makeText(PlaybackVideoActivity.this, errMessage + "[" + code + "]", Toast.LENGTH_SHORT).show();
                    }
                }
            });

        }

        @Override
        public void onJoinChannelSuccess(String channel, String userId) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    load.setVisibility(View.GONE);
                    //改变播放control的状态
                    dbyPlaybackEngine.play();
                    dbyPlaybackEngine.setSpeed(speed);
                    isPlay = true;
                    isStart = true;
                    changeControlViewBackground();
                    updateCustomEventsThrad();
                }

            });
        }

        @Override
        public void onLeaveChannel() {
            realDestory();
        }

        @Override
        public void onUserJoined(String uid) {

        }

        @Override
        public void onUserLeave(String uid) {

        }

        @Override
        public void onFirstRemoteVideoDecoded(String uid, String deviceName, int width, int height) {
            super.onFirstRemoteVideoDecoded(uid, deviceName, width, height);
            cancelRemoteVideoLoad(uid);
        }

        @Override
        public void onClientkickOff() {
            super.onClientkickOff();
        }


        @Override
        public void onRemoteVideoStateChanged(String userId, String deviceName, VideoState state) {
            super.onRemoteVideoStateChanged(userId, deviceName, state);

            Log.e("ygsdtest12311234", "onRemoteVideoStateChanged: " + userId);
            if (state == VideoState.INIT) {
                renderRemoteVideo(userId);
                remoteVideoStateChange1(userId, true, false);
            } else if (state == VideoState.STOP) {
                renderRemoveRemoteVideo(userId);
            }
        }
    };

    private void changeScreen(VideoBean videoBean) {
        if (!isPlay) {
            Toast.makeText(this, "暂停时不能切换", Toast.LENGTH_SHORT).show();
            return;
        }
        if (remotBigContainer.getChildCount() > 0) {
            VideoBean videoBeanWithSameView = getVideoBeanWithSameView((SurfaceView) remotBigContainer.getChildAt(0));
            if (videoBeanWithSameView != null) {
                videoBeanWithSameView.isBig = false;
                videoBeanWithSameView.surfaceView = dbyPlaybackEngine.createRenderView();
                FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
                layoutParams.gravity = Gravity.CENTER;
                videoBeanWithSameView.surfaceView.setLayoutParams(layoutParams);
                dbyPlaybackEngine.setVideoRender(new VideoDraw(videoBeanWithSameView.surfaceView, videoBeanWithSameView.username));
                dbyPlaybackEngine.setRemoteVideoStreamType(videoBeanWithSameView.username, 1);
                videoListAdapter.notifiInsertItme(videoBeanWithSameView);
            }
            remotBigContainer.removeViewAt(0);
        }
        videoBean.isBig = true;
        videoBean.surfaceView = dbyPlaybackEngine.createRenderView();//创建一个新的对象
        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
        layoutParams.gravity = Gravity.CENTER;
        videoBean.surfaceView.setLayoutParams(layoutParams);
        dbyPlaybackEngine.setVideoRender(new VideoDraw(videoBean.surfaceView, videoBean.username));
        dbyPlaybackEngine.setRemoteVideoStreamType(videoBean.username, 0);
        Animation anim = AnimationUtils.loadAnimation(this, R.anim.zoom_anim);
        anim.setFillAfter(true);
        remotBigContainer.startAnimation(anim);
        //        ViewUtils.removeParentChildView(videoBean.surfaceView);
        tvBigUserName.setText(videoBean.username);
        remotBigContainer.addView(videoBean.surfaceView);
//        videoListAdapter.setDataSource(videoBeanHashMap);
        videoListAdapter.notifiRemoveItem(videoBean);
        remoteVideoStateChange1(videoBean.username, videoBean.hasMount, true);

    }

    private VideoBean getVideoBeanWithSameView(SurfaceView view) {
        if (videoBeanHashMap == null) {
            return null;
        }
        if (videoBeanHashMap.isEmpty()) {
            return null;
        }
        for (HashMap.Entry<String, VideoBean> entry : videoBeanHashMap.entrySet()) {
            if (entry.getValue().surfaceView == view) {
                return entry.getValue();
            }

        }
        return null;
    }

    private ProgressBar getLoad() {
        ProgressBar bar = new ProgressBar(this);
        Drawable drawable = getDrawable(R.drawable.video_load);
        bar.setProgressDrawable(drawable);
        return bar;
    }

    private ImageView getMicView() {
        ImageView imageView = new ImageView(this);
//        imageView.setBackgroundResource(R.drawable.mic_open);
        return imageView;
    }

    private ImageView getVolumeView() {
        ImageView imageView = new ImageView(this);
        return imageView;
    }

    private void renderRemoteVideo(final String uid) {

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (isFinishing()) {
                    return;
                }
                if (videoBeanHashMap.containsKey(uid)) {
                    return;
                }
                SurfaceView surfaceView = dbyPlaybackEngine.createRenderView();
                surfaceView.setTag(uid);
                FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
                layoutParams.gravity = Gravity.CENTER;
                surfaceView.setLayoutParams(layoutParams);
                VideoBean bean = new VideoBean(uid, surfaceView, false, false, getLoad(), getMicView(), false, getVolumeView());
                bean.load.setVisibility(View.GONE);
                videoBeanHashMap.put(uid, bean);
                if (remotBigContainer.getChildCount() < 1) {
                    bean.isBig = true;
                    remotBigContainer.addView(bean.surfaceView);
                    bean.surfaceView.setVisibility(View.GONE);
//                    dbyPlaybackEngine.setRemoteVideoStreamType(bean.username, 0);
                    tvBigUserName.setText(bean.username);
                    cameraTipsImg.setVisibility(View.VISIBLE);
                    cameraTipText.setText(getString(R.string.camera_tip_str));
                } else {
                    surfaceView.setZOrderMediaOverlay(true);
//                    videoListAdapter.setDataSource(videoBeanHashMap);
                    videoListAdapter.notifiInsertItme(bean);
                }

                dbyPlaybackEngine.setVideoRender(new VideoDraw(surfaceView, bean.username));

            }
        });
    }

    private void cancelRemoteVideoLoad(final String uid) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                VideoBean bean = videoBeanHashMap.get(uid);
                Log.e("yyyyyy333", "cancelRemoteVideoLoad: " + uid + " bean=" + bean);
                if (bean == null) {
                    return;
                }
                dbyPlaybackEngine.setRemoteVideoStreamType(bean.username, bean.isBig ? 0 : 1);

                if (!bean.hasMount) {
                    //如果第一帧过来了；对应的id状态没有改变；则调整为；以第一帧到来为更新
                    remoteVideoStateChange1(bean.username, true, false);
//                    return;
                }
                if (bean.isBig) {
                    remoteVideoLoadContainer.setVisibility(View.GONE);
                }
                bean.load.setVisibility(View.GONE);
            }
        });

    }

    private void renderRemoveRemoteVideo(final String uid) {
        //判断大屏是否是该id
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                VideoBean bean = videoBeanHashMap.get(uid);
                Log.e("yyyyyy333", "renderRemoveRemoteVideo: " + uid + " bean=" + bean);
                videoBeanHashMap.remove(uid);
                if (bean == null) {
                    return;
                }
                if (bean.isBig) {
                    //移除大屏的
                    if (remotBigContainer.getChildCount() > 0) {
                        remotBigContainer.removeViewAt(0);
                        tvBigUserName.setText("");
                    }
                    //将videoBeanHashMap中的第一个元素放到大屏中
                    if (videoBeanHashMap.entrySet().iterator().hasNext()) {
                        VideoBean value = videoBeanHashMap.entrySet().iterator().next().getValue();
                        value.isBig = true;
                        tvBigUserName.setText(value.username);
                        value.surfaceView = dbyPlaybackEngine.createRenderView();//创建一个新的对象
                        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
                        layoutParams.gravity = Gravity.CENTER;
                        value.surfaceView.setLayoutParams(layoutParams);
                        dbyPlaybackEngine.setVideoRender(new VideoDraw(value.surfaceView, value.username));
                        dbyPlaybackEngine.setRemoteVideoStreamType(value.username, 0);
                        remotBigContainer.addView(value.surfaceView);
//                        videoListAdapter.setDataSource(videoBeanHashMap);
                        if (videoListAdapter != null) {
                            videoListAdapter.notifiRemoveItem(value);
                        }
                    }
                    //处理频道内一段时间内没有用户的情况
                    if (videoBeanHashMap.isEmpty()) {
                        cameraTipsImg.setVisibility(View.GONE);
                        cameraTipText.setText("频道内没有人");
                    }
                }
                if (videoListAdapter != null) {
                    videoListAdapter.notifiRemoveItem(bean);
                }
            }
        });
    }

    private void remoteVideoStateChange1(final String uid, final boolean state, final boolean change) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                VideoBean bean = videoBeanHashMap.get(uid);
                Log.e("yyyyyy333", "remoteVideoStateChange1: " + uid + " bean=" + bean + " state=" + state);
                if (bean == null) {
                    return;
                }
                bean.hasMount = state;
                if (change && state) {
                    return;
                }
                if (state) {
                    bean.surfaceView.setVisibility(View.VISIBLE);
                    bean.load.setVisibility(View.VISIBLE);
                    if (bean.isBig) {
                        remoteVideoLoadContainer.setVisibility(View.VISIBLE);
                    }
                } else {
                    if (bean.isBig) {
                        remoteVideoLoadContainer.setVisibility(View.GONE);
                    }
                    bean.surfaceView.setVisibility(View.GONE);
                    bean.load.setVisibility(View.GONE);
                }
            }
        });

    }

    private volatile long global_time;//全局同步时间
    private volatile boolean isSeeking;//是否在seek中
    private boolean isRuning = true;

    //事件更新
    private void updateCustomEventsThrad() {
        thread = new Thread(new Runnable() {
            long starttime = 0;
            long endtime = 0;

            @Override
            public void run() {
                try {
                    while (isRuning) {
                        while (isPlay) {
                            while (isSeeking) {
                                Thread.sleep((long) (UPDATE_EVENT_TIME));
                                return;
                            }
                            starttime = System.currentTimeMillis();
                            updateEvents((long) (global_time * speed));
                            refreshChatAdapter();
                            Thread.sleep((long) (UPDATE_EVENT_TIME));
                            endtime = System.currentTimeMillis();
                            global_time += endtime - starttime;
                        }
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        });
        thread.start();
    }

    ChatMessage message;
    private String uid;
    private long msgTime;
    private long lastNowTime = 0;
    private int lastListSize = 0;

    private void updateEvents(long nowTime) {
        if (customEventList == null) {
            return;
        }
        for (CustomEventBean bean : customEventList) {
            if (nowTime + mCourseStartTime > mCourseStartTime + mTotalTime) {//已经大于课程的总时间了
                return;
            }
            if (bean.getRecordTime() > nowTime + mCourseStartTime) {
                return;
            }
            if (bean.getUid().equals(uid) && bean.getRecordTime() == msgTime) {
                return;
            }
            uid = bean.getUid();
            msgTime = bean.getRecordTime();
            message = new ChatMessage(bean.getUid(), bean.getContent(), bean.getRecordTime());
            messageList.add(message);
        }
    }

    Set<ChatMessage> messADSet;

    private void refreshChatAdapter() {
//        messageListAD.addAll(messageList);
        if (lastListSize == messageList.size()) {
            return;
        }

        if (messADSet != null) {
            if (messADSet.containsAll(messageList)) {
                return;
            }
        }
        messageListAD.clear();
        messageListAD = (ArrayList<ChatMessage>) messageList.clone();
        messADSet = new HashSet<>(messageListAD);
        lastListSize = messageList.size();
        messageList.clear();
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (messageListAD.size() > 0) {
                    chatAdapter.setList(messageListAD);
                    chatAdapter.notifyDataSetChanged();
                    int itemCount = chatAdapter.getItemCount();
                    if (itemCount > 1) {
                        mUserOnlineView.smoothScrollToPosition(itemCount - 1);
                    }
                }
            }
        });
    }


    public static void startActivity(Context context) {
        Intent intent = new Intent(context, PlaybackVideoActivity.class);
        context.startActivity(intent);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            showTipsDialog();
        }
        return super.onKeyDown(keyCode, event);
    }

    private void showTipsDialog() {
        showTipsDialog("确定要退出吗？", 1);
    }

    private void showTipsDialog(String msg, int type) {
        //type=0只显示确定按钮
        final TipsDialog dialog = new TipsDialog(PlaybackVideoActivity.this);
        dialog.setMessage(msg);

        dialog.setYesOnclickListener("确定", new TipsDialog.onYesOnclickListener() {
            @Override
            public void onYesClick() {
                stopDbyPlayEngine();
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
        dialog.setCancelShow(type == 1);
    }

    private void realDestory() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {

                if (load.getVisibility() == View.VISIBLE) {
                    load.setVisibility(View.GONE);
                }
                if (PlaybackVideoActivity.this.isFinishing()) {
                    return;
                }
                PlaybackVideoActivity.this.finish();
            }
        });
    }

    private void changeControlViewBackground() {
        if (isPlay) {
            controBtn.setBackgroundResource(R.drawable.playback_play);
        } else {
            controBtn.setBackgroundResource(R.drawable.playback_stop);
        }
    }
}
