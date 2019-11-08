package com.duobeiyun.demo_1v1;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.duobeiyun.demo_1v1.widget.CustomDatePicker;
import com.duobeiyun.demo_1v1.widget.DateFormatUtils;
import com.duobeiyun.demo_1v1.widget.RecordIdBean;
import com.duobeiyun.paassdk.utils.DbyEngineConstant;
import com.duobeiyun.paassdk.utils.JsonUtils;
import com.duobeiyun.paassdk.utils.OkhttpUtils;

public class GeneratePlaybackActivity extends AppCompatActivity implements View.OnClickListener {

    private Button btnJoinPlayback, btnGenerate;
    private EditText et_channelId, te_record_id;
    private CustomDatePicker mTimerPicker, mTimerEndPicker;
    private TextView mTvSelectedDate, mTvSelectedEndTime;
    private ProgressBar load;
    private long recordStartTime;
    private long recordEndTime;
    private String recordId;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_generate_playback);
        btnJoinPlayback = findViewById(R.id.btn_join);
        btnGenerate = findViewById(R.id.btn_generate);
        et_channelId = findViewById(R.id.te_channel_id);
        te_record_id = findViewById(R.id.te_record_id);
        load = findViewById(R.id.load);
        btnJoinPlayback.setOnClickListener(this);
        btnGenerate.setOnClickListener(this);
        findViewById(R.id.ll_start_date).setOnClickListener(this);
        findViewById(R.id.ll_end_date).setOnClickListener(this);
        mTvSelectedDate = findViewById(R.id.tv_selected_date);
        mTvSelectedEndTime = findViewById(R.id.tv_selected_end_date);

        String recordChannelId = SharePreUtils.getInstance(this.getApplicationContext()).getValue(Constant.CHANNELID, "");
        recordStartTime = SharePreUtils.getInstance(this.getApplicationContext()).getValue(Constant.CHANNEL_START_TIME, System.currentTimeMillis());
        recordEndTime = SharePreUtils.getInstance(this.getApplicationContext()).getValue(Constant.CHANNEL_END_TIME, System.currentTimeMillis());
        recordId = SharePreUtils.getInstance(this.getApplicationContext()).getValue(Constant.CHANNEL_RECORD_ID, "");
        te_record_id.setText(recordChannelId);

        initTimerPicker(mTvSelectedDate);
        initTimerEndPicker(mTvSelectedEndTime);

        te_record_id.setText(recordId);
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.ll_start_date:
                // 日期格式为yyyy-MM-dd
                mTimerPicker.show(mTvSelectedDate.getText().toString());
                break;

            case R.id.ll_end_date:
                // 日期格式为yyyy-MM-dd HH:mm
                mTimerEndPicker.show(mTvSelectedEndTime.getText().toString());
                break;
            case R.id.btn_generate:
                generatePlayBack();
                break;
            case R.id.btn_join:
                openPlaybackVideoActivity();
                break;
        }
    }

    private void openPlaybackVideoActivity() {
        recordId = te_record_id.getText().toString();
        if (null == recordId || "".equals(recordId)) {
            Toast.makeText(this, "recordId不能为空", Toast.LENGTH_SHORT).show();
            return;
        }
        SharePreUtils.getInstance(this.getApplicationContext()).setValue(Constant.CHANNEL_RECORD_ID, recordId);

        PlaybackVideoActivity.startActivity(this);
    }

    private void generatePlayBack() {

        String playChannelId = et_channelId.getText().toString();
        String startTime = mTvSelectedDate.getText().toString() + ":00";
        String endTime = mTvSelectedEndTime.getText().toString() + ":00";
        String generateRecordIdUrl = DbyEngineConstant.generateRecordId(playChannelId, startTime,
                endTime, getString(R.string.app_id), getString(R.string.live_appkey));

        long l = DateFormatUtils.str2Long(startTime, true);
        long l1 = DateFormatUtils.str2Long(endTime, true);
        if (playChannelId == null || "".equals(playChannelId)) {
            Toast.makeText(this, "生成回放的频道id不能为空", Toast.LENGTH_SHORT).show();
            return;
        }
        if (l1 < l) {
            Toast.makeText(this, "结束时间必须大于或等于开始时间", Toast.LENGTH_SHORT).show();
            return;
        }
        load.setVisibility(View.VISIBLE);
        OkhttpUtils.getInstance().post(generateRecordIdUrl, new OkhttpUtils.ResultCallback<String>() {
            @Override
            public void onSuccess(String data) {
                final RecordIdBean recordIdBean = JsonUtils.GsonToBean(data, RecordIdBean.class);
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        if (recordIdBean != null) {
                            if (recordIdBean.getCode() == 200) {
                                Toast.makeText(GeneratePlaybackActivity.this, "recordId生成成功", Toast.LENGTH_SHORT).show();
                                if (recordIdBean.getData() == null) {
                                    return;
                                }
                                if (recordIdBean.getData().getRecordId() == null) {
                                    return;
                                }
                                te_record_id.setText(recordIdBean.getData().getRecordId());
                            } else {
                                Toast.makeText(GeneratePlaybackActivity.this, "recordId生成出现错误[" + recordIdBean.getMsg() + "]", Toast.LENGTH_SHORT).show();
                            }
                        }
                        load.setVisibility(View.GONE);
                    }
                });
            }

            @Override
            public void onFail(final Exception e) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(GeneratePlaybackActivity.this, "生成回方recordId失败[" + (e.getMessage()) + "]", Toast.LENGTH_SHORT).show();
                        load.setVisibility(View.GONE);
                    }
                });
            }
        });
    }

    private void initTimerPicker(final TextView textView) {
        String beginTime = "2019-01-01 01:00";
        String endTime = DateFormatUtils.long2Str(System.currentTimeMillis(), true);
        String courstarttime = DateFormatUtils.long2Str(recordStartTime, true);

        textView.setText(courstarttime);

        // 通过日期字符串初始化日期，格式请用：yyyy-MM-dd HH:mm
        mTimerPicker = new CustomDatePicker(this, new CustomDatePicker.Callback() {
            @Override
            public void onTimeSelected(long timestamp) {
                textView.setText(DateFormatUtils.long2Str(timestamp, true));
            }
        }, beginTime, endTime);
        // 允许点击屏幕或物理返回键关闭
        mTimerPicker.setCancelable(true);
        // 显示时和分
        mTimerPicker.setCanShowPreciseTime(true);
        // 允许循环滚动
        mTimerPicker.setScrollLoop(true);
        // 允许滚动动画
        mTimerPicker.setCanShowAnim(true);
    }

    private void initTimerEndPicker(final TextView textView) {
        String beginTime = "2019-01-01 01:00";
        String endTime = DateFormatUtils.long2Str(System.currentTimeMillis(), true);
        String courseEndTime = DateFormatUtils.long2Str(recordEndTime, true);
        textView.setText(courseEndTime);

        // 通过日期字符串初始化日期，格式请用：yyyy-MM-dd HH:mm
        mTimerEndPicker = new CustomDatePicker(this, new CustomDatePicker.Callback() {
            @Override
            public void onTimeSelected(long timestamp) {
                textView.setText(DateFormatUtils.long2Str(timestamp, true));
            }
        }, beginTime, endTime);
        // 允许点击屏幕或物理返回键关闭
        mTimerEndPicker.setCancelable(true);
        // 显示时和分
        mTimerEndPicker.setCanShowPreciseTime(true);
        // 允许循环滚动
        mTimerEndPicker.setScrollLoop(true);
        // 允许滚动动画
        mTimerEndPicker.setCanShowAnim(true);
    }

    public static void openGeneratePlaybackActivity(Context context) {
        Intent intent = new Intent(context, GeneratePlaybackActivity.class);
        context.startActivity(intent);
    }
}
