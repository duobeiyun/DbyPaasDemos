package com.duobeiyun.demo_1v1;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;


import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.List;

public class ChatAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {

    public static final int TYPE_USER_ONLINE = 0;
    public static final int TYPE_USER_CHAT = 1;

    private List<ChatMessage> list;

    public ChatAdapter(List<ChatMessage> list) {
        this.list = list;
    }

    public void setList(List<ChatMessage> list) {
        if (this.list != null) {
            this.list.clear();
        }
        this.list.addAll(list);
    }

    @NonNull
    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int i) {
            View view = LayoutInflater.from(viewGroup.getContext()).inflate(R.layout.chat_adapter_layout, viewGroup, false);
            return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull RecyclerView.ViewHolder viewHolder, int i) {
        if (viewHolder instanceof ViewHolder) {
            ViewHolder holder = (ViewHolder) viewHolder;
            holder.tv_name.setText(list.get(i).userId + ": ");
            holder.tv_msg.setText(list.get(i).msg + "");
        } else {
            OnLineViewHolder onLineViewHolder = (OnLineViewHolder) viewHolder;
            onLineViewHolder.tv_name.setText("用户" + list.get(i).userId + (list.get(i).isOnline ? "上线" : "下线"));
        }
    }

    @Override
    public int getItemCount() {
        return list.size();
    }

    @Override
    public int getItemViewType(int position) {
        if (list == null) {
            return 0;
        }
        ChatMessage chatMessage = list.get(position);
        if (chatMessage.type == 0) {
            return TYPE_USER_ONLINE;
        } else {
            return TYPE_USER_CHAT;
        }
    }

    public class ViewHolder extends RecyclerView.ViewHolder {
        public TextView tv_name;
        public TextView tv_msg;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            tv_name = itemView.findViewById(R.id.tv_name);
            tv_msg = itemView.findViewById(R.id.tv_mesg);
        }
    }

    public class OnLineViewHolder extends RecyclerView.ViewHolder {
        public TextView tv_name;

        public OnLineViewHolder(@NonNull View itemView) {
            super(itemView);
            tv_name = itemView.findViewById(R.id.tv_name);
        }
    }
}
