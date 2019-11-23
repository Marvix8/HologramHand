package com.unlam.hologramhand;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.CountDownTimer;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class CalibrationHandActivity extends AppCompatActivity {

    private Button startButton;
    private TextView instructionText;
    private boolean firstConfigurationDone;
    private boolean configurationDone;
    private long startTime;
    private long interval;
    private long sendInstructionTime;
    private BluetoothService bluetoothService;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_calibration_hand);
        startButton = (Button) findViewById(R.id.startConfiguration);
        instructionText = (TextView) findViewById(R.id.instructionText);
        this.firstConfigurationDone = false;
        this.configurationDone = false;
        this.sendInstructionTime = Long.valueOf(getString(R.string.two_seconds));
        this.interval = Long.valueOf(getString(R.string.one_second));
        this.startTime = Long.valueOf(getString(R.string.ten_seconds));
        this.bluetoothService = BluetoothService.create();
//        this.interval = 1000;
//        this.startTime = 10000;
//        this.sendInstructionTime = 2;
        this.bluetoothService = BluetoothService.create();
    }

    public void startCounter() {
        new CountDownTimer(this.startTime, this.interval) {

            long currentTime;

            public void onTick(long millisUntilFinished) {
                currentTime = millisUntilFinished / CalibrationHandActivity.this.interval;
                startButton.setText(String.valueOf(currentTime));
                if (currentTime == CalibrationHandActivity.this.sendInstructionTime) {
                    try {
                        if (!firstConfigurationDone) {
                            CalibrationHandActivity.this.bluetoothService.write("B");
                        } else {
                            CalibrationHandActivity.this.bluetoothService.write("S");
                        }
                    } catch (NullPointerException ex) {
                        ToasterPrinter.printToasterLong("El dipositivo no está conectado a bluetooth", CalibrationHandActivity.this);
                        finish();
                    }

                }
            }

            public void onFinish() {

                if (!CalibrationHandActivity.this.firstConfigurationDone && !CalibrationHandActivity.this.configurationDone) {
                    CalibrationHandActivity.this.firstConfigurationDone = true;
                    startButton.setText(R.string.start_configuration);
                    CalibrationHandActivity.this.instructionText.setText(R.string.second_configuration_message);
                } else if (CalibrationHandActivity.this.firstConfigurationDone && !CalibrationHandActivity.this.configurationDone) {
                    CalibrationHandActivity.this.configurationDone = true;
                    startButton.setText(R.string.finish_configuration);
                    CalibrationHandActivity.this.instructionText.setText(R.string.configuration_done_message);
                }
                startButton.setEnabled(true);
            }
        }.start();
    }

    public void startConfiguration(View view) {
        startButton.setEnabled(false);
        if (!this.configurationDone) {
            startCounter();

        } else {
            finish();
        }
    }


}
