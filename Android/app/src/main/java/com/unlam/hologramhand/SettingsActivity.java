package com.unlam.hologramhand;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;

public class SettingsActivity extends AppCompatActivity implements SensorEventListener {

    private int minValue = 0;
    private int maxValue = 500;
    private int current = 1;
    private SeekBar seekBar;
    private TextView seekBarIndicator;
    private TextView environmentalLight;
    private SensorManager sensorManager;
    private Sensor light;
    private int zero;


    public void openCalibrationHand(View view) {
        Intent intent = new Intent(this, CalibrationHandActivity.class);
        startActivity(intent);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        if (SharedPreferencesStatic.existPreference(this, getString(R.string.settings_shared_preferences), getString(R.string.shared_preferences_bright))) {
            this.current = SharedPreferencesStatic.getIntPreference(this, getString(R.string.settings_shared_preferences), getString(R.string.shared_preferences_bright));
        }else{
            this.current = Integer.valueOf(getString(R.string.environment_light));
        }

        this.zero = Integer.valueOf(getString(R.string.zero));
        seekBar = (SeekBar) findViewById(R.id.seekBar);
        seekBarIndicator = (TextView) findViewById(R.id.percentage_bright);
        environmentalLight = (TextView) findViewById(R.id.environmental_bright);
        seekBar.setMax(this.maxValue - this.minValue);
        seekBar.setProgress(this.current - this.minValue);
        seekBarIndicator.setText(String.valueOf(this.current));

        this.initializeSensors();

        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {

            public void onStopTrackingTouch(SeekBar seekBar) {

            }

            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                SettingsActivity.this.current = progress + SettingsActivity.this.minValue;
                seekBarIndicator.setText(String.valueOf(SettingsActivity.this.current));
            }
        });
    }

    private void initializeSensors() {
        this.sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        this.light = sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT);

        this.sensorManager.registerListener(this, light, SensorManager.SENSOR_DELAY_NORMAL);
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        switch (event.sensor.getType()) {
            case Sensor.TYPE_LIGHT:
                this.environmentalLight.setText(String.valueOf(event.values[this.zero]));
                break;
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    public void savePreferences(View view) {
        SharedPreferences.Editor editor = getSharedPreferences(getString(R.string.settings_shared_preferences), MODE_PRIVATE).edit();
        editor.putInt(getString(R.string.shared_preferences_bright), this.current);
        editor.apply();
        ToasterPrinter.printToasterShort("Se guardó correctamente", this);
    }

    public boolean existPreference(String preference) {
        SharedPreferences sharedPreferences = getSharedPreferences(getString(R.string.settings_shared_preferences), MODE_PRIVATE);
        int value = sharedPreferences.getInt(preference, -1);
        return value != -1;
    }

    public int getIntPreference(String preference) {
        SharedPreferences sharedPreferences = getSharedPreferences(getString(R.string.settings_shared_preferences), MODE_PRIVATE);
        return sharedPreferences.getInt(preference, -1);
    }


}
