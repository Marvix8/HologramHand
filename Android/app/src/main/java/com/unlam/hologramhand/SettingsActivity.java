package com.unlam.hologramhand;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;

public class SettingsActivity extends AppCompatActivity {

    private int minValue = 0;
    private int maxValue = 50;
    private int current = 10;
    private SeekBar seekBar;
    private TextView seekBarIndicator;

    public void openCalibrationHand(View view) {
        Intent intent = new Intent(this, CalibrationHandActivity.class);
        startActivity(intent);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);
//
//        seekBar = (SeekBar) findViewById(R.id.seekBar);
//        seekBarIndicator = (TextView) findViewById(R.id.percentage_bright);
//        seekBar.setMax(this.maxValue - this.minValue);
//        seekBar.setProgress(this.current - this.minValue);
//        seekBarIndicator.setText(String.valueOf(this.current));
//
//        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
//
//            public void onStopTrackingTouch(SeekBar seekBar) {
//
//            }
//
//            public void onStartTrackingTouch(SeekBar seekBar) {
//            }
//
//            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
//                SettingsActivity.this.current = progress + SettingsActivity.this.minValue;
//                seekBarIndicator.setText(String.valueOf(SettingsActivity.this.current));
//            }
//        });
    }


}
