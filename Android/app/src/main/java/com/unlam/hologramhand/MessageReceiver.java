package com.unlam.hologramhand;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class MessageReceiver extends BroadcastReceiver {

    private Context context;
    private VideoPlayer videoPlayer = null;
    private final int oneSecond;
    private final int start;
    private final int defaultSkipTime;
    private final int zero;
    private final int play;
    private final int pause;
    private final int stop;
    private final int skipF;
    private final int skipB;
    private final int skipSF;
    private final String instruction;

    public MessageReceiver(Context context) {
        this.context = context;
        this.oneSecond = Integer.valueOf(this.context.getString(R.string.one_second));
        this.start = Integer.valueOf(this.context.getString(R.string.start));
        this.defaultSkipTime = Integer.valueOf(this.context.getString(R.string.default_skip_time));
        this.zero = Integer.valueOf(this.context.getString(R.string.zero));
        this.play = Integer.valueOf(this.context.getString(R.string.play));
        this.pause = Integer.valueOf(this.context.getString(R.string.pause));
        this.stop = Integer.valueOf(this.context.getString(R.string.stop));
        this.skipF = Integer.valueOf(this.context.getString(R.string.skip_forward));
        this.skipB = Integer.valueOf(this.context.getString(R.string.skip_backward));
        this.skipSF = Integer.valueOf(this.context.getString(R.string.skip_super_forward));
        this.instruction = this.context.getString(R.string.instruction);
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        String instruction = intent.getStringExtra(this.instruction);
        this.selectInstruction(instruction);
    }

    private void selectInstruction(String instruction) {
        if(this.videoPlayer.isAlertOpen()){
            return;
        }
        if(instruction.equals(this.play)){
            this.performPlay();
        }else if(instruction.equals(this.pause)){
            this.performPause();
        }else if(instruction.equals(this.stop)){
            this.performStop();
        }else if(instruction.equals(this.skipF)){
            this.skipForward(this.defaultSkipTime);
        }else if(instruction.equals(this.skipSF)){
            this.skipForward(this.defaultSkipTime + this.defaultSkipTime);
        }else if(instruction.equals(this.skipB)){
            this.skipBackward(this.defaultSkipTime);
        }else{
            this.showOnConsole(instruction);
        }
    }

    private void showOnConsole(String message){
        System.out.println(message);
    }

    public void setActivityContext(VideoPlayer videoPlayer) {
        this.videoPlayer = videoPlayer;
    }

    private void performPause() {
        this.videoPlayer.getVideoView().pause();
    }

    private void performPlay() {
        this.videoPlayer.getVideoView().start();
    }

    private void performStop() {
        this.videoPlayer.getVideoView().pause();
        seekTo(this.zero);
    }

    private void skipForward(int seconds) {
        if (isPositive(seconds)) {
            final int currentPosition = this.videoPlayer.getVideoView().getCurrentPosition();
            final int newPosition = currentPosition + (seconds * this.oneSecond);
            final int duration = this.videoPlayer.getVideoView().getDuration();
            if (newPosition > duration) {
                seekTo(duration);
            } else {
                seekTo(newPosition);
            }
            this.performPlay();
        }
    }

    private void skipBackward(int seconds) {
        if (isPositive(seconds)) {
            final int currentPosition = this.videoPlayer.getVideoView().getCurrentPosition();
            final int newPosition = currentPosition - (seconds * this.oneSecond);
            if (isPositive(newPosition)) {
                seekTo(newPosition);
            } else {
                seekTo(this.start);
            }
            this.performPlay();
        }
    }

    private boolean isPositive(int value) {
        return value >= this.zero;
    }

    private void seekTo(int milliseconds) {
        this.videoPlayer.getVideoView().seekTo(milliseconds);
    }
}
