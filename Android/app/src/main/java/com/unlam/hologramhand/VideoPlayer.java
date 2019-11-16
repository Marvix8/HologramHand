package com.unlam.hologramhand;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.Uri;
import android.os.Bundle;
import android.view.WindowManager;
import android.widget.VideoView;
import androidx.appcompat.app.AppCompatActivity;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

public class VideoPlayer extends AppCompatActivity implements SensorEventListener {

    private AlertDialog alertDialog;
    private SensorManager sensorManager;
    private Sensor light;
    private Sensor accelerometer;
    private boolean alertDialogOpen = false;
    private VideoView videoView;
    private MessageReceiver mMessageReceiver;
    private String resourceBaseURI;
    private String slash;
    private String gestureInstruction;
    private int zero;
    private float accelerometerZAxisGravity;
    private float environmentalLight;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
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

        this.initializeSensors();

        LocalBroadcastManager.getInstance(this).registerReceiver(mMessageReceiver, new IntentFilter(this.gestureInstruction));
    }

    @Override
    protected void onDestroy() {
        LocalBroadcastManager.getInstance(this).unregisterReceiver(mMessageReceiver);
        this.alertDialog.dismiss();
        this.alertDialogOpen = false;
        super.onDestroy();
    }

    @Override
    public final void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    @Override
    public final void onSensorChanged(SensorEvent event) {
        switch (event.sensor.getType()) {
            case Sensor.TYPE_ACCELEROMETER:
                if (exceedMinimumZAxis(event.values[SensorManager.DATA_Z]) && !this.alertDialogOpen) {
                    this.openAlertDialog();
                }
                break;
            case Sensor.TYPE_LIGHT:
                if (isBright(event.values[this.zero]) && !this.alertDialogOpen) {
                    this.openAlertDialog();
                }
                break;
        }
    }

    private boolean isBright(float value) {
        return value != this.environmentalLight;
    }

    private boolean exceedMinimumZAxis(float value) {
        return value <= this.accelerometerZAxisGravity;
    }

    private void initializeSensors() {
        this.sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        this.light = sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT);
        this.accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);

        this.sensorManager.registerListener(this, light, SensorManager.SENSOR_DELAY_NORMAL);
        this.sensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_NORMAL);
    }

    public VideoView getVideoView() {
        return videoView;
    }

    public boolean isAlertOpen() {
        return this.alertDialogOpen;
    }

    private void initializeVariables() {
        this.resourceBaseURI = getString(R.string.resource_base_URI);
        this.slash = getString(R.string.slash);
        this.gestureInstruction = getString(R.string.gesture_instruction);
        this.zero = Integer.valueOf(getString(R.string.zero));
        this.accelerometerZAxisGravity = Float.valueOf(getString(R.string.accelerometer_z_axis_gravity));
        this.environmentalLight = Float.valueOf(R.string.environment_light);
    }

    private String getURIVideo() {
        return this.resourceBaseURI + getPackageName() + this.slash + R.raw.holograma;
    }

    private void openAlertDialog() {
        this.alertDialogOpen = true;
        this.videoView.pause();
        this.alertDialog = this.createAlertDialog().show();

    }

    private AlertDialog.Builder createAlertDialog() {
        return new AlertDialog.Builder(this)
                .setMessage(R.string.alert_dialog_message).setTitle(R.string.alert_dialog_title)
                .setPositiveButton(R.string.accept_alert_dialog, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.dismiss();
                        VideoPlayer.this.alertDialogOpen = false;
                    }
                });
    }
}
