package com.unlam.hologramhand;

import android.content.IntentFilter;
import android.content.res.Resources;
import android.net.Uri;
import android.os.Bundle;
import android.view.WindowManager;
import android.widget.VideoView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

public class VideoPlayer extends AppCompatActivity {

    private VideoView videoView;
    private MessageReceiver mMessageReceiver;
    private String resourceBaseURI;
    private String slash;
    private String gestureInstruction;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.initializeVariables();
        this.mMessageReceiver = new MessageReceiver(this);
        this.mMessageReceiver.setActivityContext(this);
        getSupportActionBar().hide(); //OCULTA BARRA DE TITLE ACTIVITY
        setContentView(R.layout.activity_video_player);

        this.videoView = (VideoView) findViewById(R.id.video_player);
        Uri uri = Uri.parse(this.getURIVideo());
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN); //OCULTA BARRA DE ESTADO ANDROID

        videoView.setVideoURI(uri);
        videoView.requestFocus();
        videoView.start();

        LocalBroadcastManager.getInstance(this).registerReceiver(mMessageReceiver, new IntentFilter(this.gestureInstruction));
    }

    @Override
    protected void onDestroy() {
        LocalBroadcastManager.getInstance(this).unregisterReceiver(mMessageReceiver);
        super.onDestroy();
    }

    public VideoView getVideoView() {
        return videoView;
    }

    private void initializeVariables() {
        this.resourceBaseURI = getString(R.string.resource_base_URI);
        this.slash = getString(R.string.slash);
        this.gestureInstruction = getString(R.string.gesture_instruction);
    }

    private String getURIVideo() {
        return this.resourceBaseURI + getPackageName() + this.slash + R.raw.holograma;
    }
}
