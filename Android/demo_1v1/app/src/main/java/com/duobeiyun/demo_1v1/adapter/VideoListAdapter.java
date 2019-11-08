package com.duobeiyun.demo_1v1.adapter;

import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.duobeiyun.demo_1v1.R;
import com.duobeiyun.demo_1v1.bean.VideoBean;
import com.duobeiyun.demo_1v1.utils.ViewUtils;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class VideoListAdapter extends RecyclerView.Adapter<VideoListAdapter.ViewHolder> {

    private HashMap<String, VideoBean> videoMap = new HashMap<>();

    private List<VideoBean> list = new ArrayList<>();

    private VideoViewChangeListener listener;

    public VideoListAdapter(HashMap<String, VideoBean> videoMap) {
        this.videoMap.putAll(videoMap);
    }

    public void setVideoViewChangeListener(VideoViewChangeListener plistener) {
        this.listener = plistener;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int i) {
        View view = LayoutInflater.from(viewGroup.getContext()).inflate(R.layout.video_list_adapter, viewGroup, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder viewHolder, final int i) {
        if (viewHolder.fl_container.getChildCount() > 0) {
            viewHolder.fl_container.removeAllViews();
        }
        final VideoBean videoBean = list.get(i);
        ViewUtils.removeParentChildView(videoBean.surfaceView);
        videoBean.surfaceView.setZOrderMediaOverlay(true);
        viewHolder.fl_container.addView(videoBean.surfaceView);
        ViewUtils.removeParentChildView(videoBean.load);
        ViewUtils.removeParentChildView(videoBean.micView);
        ViewUtils.removeParentChildView(videoBean.volumeView);
        //video load
        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
        layoutParams.gravity = Gravity.CENTER;
        videoBean.load.setLayoutParams(layoutParams);
        viewHolder.fl_container.addView(videoBean.load);
        //mic tip
        FrameLayout.LayoutParams miclayoutParams = new FrameLayout.LayoutParams(32, 32);
        miclayoutParams.gravity = Gravity.RIGHT;
        miclayoutParams.setMargins(0, 5, 5, 0);
        viewHolder.fl_container.addView(videoBean.micView);
        videoBean.micView.setLayoutParams(miclayoutParams);

        FrameLayout.LayoutParams volumelayoutParams = new FrameLayout.LayoutParams(64, 64);
        volumelayoutParams.setMargins(0, 80, 0, 0);
        viewHolder.fl_container.addView(videoBean.volumeView);
        videoBean.volumeView.setLayoutParams(volumelayoutParams);

        if (videoBean.hasMount) {
            videoBean.surfaceView.setVisibility(View.VISIBLE);
        } else {
            videoBean.surfaceView.setVisibility(View.GONE);
        }
        viewHolder.tv_name.setText(videoBean.username);
        viewHolder.ib_change.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (listener != null) {
                    listener.onItemChange(videoBean);
                }
            }
        });
        videoBean.surfaceView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (listener != null) {
                    listener.surfaceClick(videoBean);

                }
            }
        });

    }


    @Override
    public int getItemCount() {
        return list.size();
    }


    public void setDataSource(HashMap<String, VideoBean> map) {
        this.videoMap.clear();
        this.videoMap.putAll(map);
        list.clear();
        for (HashMap.Entry<String, VideoBean> entry : videoMap.entrySet()) {
            if (entry.getValue() == null) {
                continue;
            }
            if (!entry.getValue().isBig) {
                list.add(entry.getValue());
            }
        }
        this.notifyDataSetChanged();
    }

    private void notifyDataChangeItem(VideoBean bean) {
        int videoPosition = getVideoPosition(bean);
        if (videoPosition != -1) {
            this.notifyItemChanged(videoPosition);
        }
    }

    private void changePosition(int videoPosition) {
        if (videoPosition != list.size()) {
            notifyItemRangeChanged(videoPosition, list.size() - videoPosition);
        }
    }

    public void notifiInsertItme(VideoBean bean) {
        if (isSameBean(bean)) {
            notifyDataChangeItem(bean);
        } else {
            list.add(bean);
            notifyItemInserted(getVideoPosition(bean));
            changePosition(getVideoPosition(bean));
        }
    }

    public void notifiRemoveItem(VideoBean bean) {
        int videoPosition = getVideoPosition(bean);
        if (videoPosition != -1) {
            this.notifyItemRemoved(videoPosition);
            changePosition(videoPosition);
            list.remove(bean);
        }
    }

    private boolean isSameBean(VideoBean bean) {
        for (int i = 0; i < list.size(); i++) {
            if (bean == list.get(i)) {
                return true;
            }
        }
        return false;
    }

    private int getVideoPosition(VideoBean bean) {
        for (int i = 0; i < list.size(); i++) {
            if (bean.username.equals(list.get(i).username)) {
                return i;
            }
        }
        return -1;
    }

    public class ViewHolder extends RecyclerView.ViewHolder {
        public TextView tv_name;
        public ImageButton ib_change;
        public FrameLayout fl_container;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            tv_name = itemView.findViewById(R.id.tv_username);
            ib_change = itemView.findViewById(R.id.ib_change);
            fl_container = itemView.findViewById(R.id.fl_container);
        }
    }


}
