package com.unlam.hologramhand;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class MessageReceiver  extends BroadcastReceiver {

    private VideoPlayer videoPlayer = null;
    @Override
    public void onReceive(Context context, Intent intent) {
        // Get extra data included in the Intent
        String instruction = intent.getStringExtra("instruction");
//        VideoPlayer activity = (VideoPlayer) context;
        this.videoPlayer.getVideoView().pause();
        switch (instruction){
            case "P":

                break;
        }
    }

    void setActivityContext(VideoPlayer videoPlayer){
        this.videoPlayer=videoPlayer;
    }
}
