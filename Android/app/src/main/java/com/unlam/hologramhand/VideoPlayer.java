package com.unlam.hologramhand;

import android.net.Uri;
import android.os.Bundle;
import android.widget.MediaController;
import android.widget.VideoView;

import androidx.appcompat.app.AppCompatActivity;

public class VideoPlayer extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video_player);
        VideoView videoPlayer = (VideoView) findViewById(R.id.video_player);
        String videoPath = "android.resource\\" +getPackageName()+ "Android\\app\\src\\main\\res\\raw\\holograma";
        Uri uri = Uri.parse(videoPath);
        //videoPlayer.setVideoPath("");
        videoPlayer.setVideoURI(uri);
        MediaController mediaController = new MediaController(this);
        videoPlayer.setMediaController(mediaController);
        mediaController.setAnchorView(videoPlayer);
    }
}
