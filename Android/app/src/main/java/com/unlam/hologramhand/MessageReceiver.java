package com.unlam.hologramhand;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class MessageReceiver extends BroadcastReceiver {

    private VideoPlayer videoPlayer = null;
    private final int milisecs = 1000;
    private final int start = 0;
    private final int defaultSkipTime = 10;
    private final int zero = 0;
    private final String play = "A";
    private final String pause = "B";
    private final String stop = "C";
    private final String skipF = "D";
    private final String skipB = "E";
    private final String instruction = "instruction";

    @Override
    public void onReceive(Context context, Intent intent) {
        String instruction = intent.getStringExtra(this.instruction);
        this.videoPlayer.getVideoView().pause();
        switch (instruction) {
            case play:
                this.performPlay();
                break;
            case pause:
                this.performPause();
                break;
            case stop:
                this.performStop();
                break;
            case skipF:
                this.skipForward(this.defaultSkipTime);
                break;
            case skipB:
                this.skipBackward(this.defaultSkipTime);
                break;
        }
    }

    void setActivityContext(VideoPlayer videoPlayer) {
        this.videoPlayer = videoPlayer;
    }

    private void performPause() {
        if (this.videoPlayer.getVideoView().canPause()) {
            this.videoPlayer.getVideoView().pause();
        }
    }

    private void performPlay() {
        this.videoPlayer.getVideoView().pause();
    }

    private void performStop() {
        this.videoPlayer.getVideoView().pause();
    }

    private void skipForward(int seconds) {
        if (isPositive(seconds)) {
            final int currentPosition = this.videoPlayer.getVideoView().getCurrentPosition();
            final int newPosition = currentPosition + (seconds * this.milisecs);
            final int duration = this.videoPlayer.getVideoView().getDuration();
            if (newPosition > duration) {
                seekTo(duration);
            } else {
                seekTo(newPosition);
            }
        }
    }

    private void skipBackward(int seconds) {
        if (isPositive(seconds)) {
            final int currentPosition = this.videoPlayer.getVideoView().getCurrentPosition();
            final int newPosition = currentPosition - (seconds * this.milisecs);
            if (isPositive(newPosition)) {
                seekTo(newPosition);
            } else {
                seekTo(this.start);
            }
        }
    }

    private boolean isPositive(int value) {
        return value >= this.zero;
    }

    private void seekTo(int milisecs) {
        this.videoPlayer.getVideoView().seekTo(milisecs);
    }
}
