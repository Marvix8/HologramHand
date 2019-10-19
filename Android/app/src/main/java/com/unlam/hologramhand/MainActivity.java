package com.unlam.hologramhand;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.CompoundButton;
import android.widget.ListView;
import android.widget.Switch;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Set;

public class MainActivity extends AppCompatActivity {

    private Switch bluetoothSwitch;
    private Set<BluetoothDevice> pairedDevices;
    private ListView deviceList;
    private ArrayAdapter adapter;
    private BluetoothAdapter bluetoothAdapter;
    public static final int REQUEST_CODE = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        bluetoothSwitch = (Switch) findViewById(R.id.bluetoothSwitch);
        validateBluetoothState();
        if (bluetoothSwitch != null) {
            bluetoothSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                    if (isChecked) {
                        this.turnOnBluetooth();
                    } else {
                        this.turnOffBluetooth();
                    }
                }

                public void turnOnBluetooth(){
                    if (!bluetoothAdapter.isEnabled()) {
                        Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                        startActivityForResult(turnOn, REQUEST_CODE);
                        Toast.makeText(getApplicationContext(), "Activando",Toast.LENGTH_SHORT).show();
                    } else {
                        this.list();
                    }
                }
                public void list() {
                    pairedDevices = bluetoothAdapter.getBondedDevices();
                    deviceList = (ListView) findViewById(R.id.listView);
                    ArrayList list = new ArrayList();

                    for (BluetoothDevice bt : pairedDevices) {
                        list.add(bt.getName());
                    }
                    Toast.makeText(getApplicationContext(), "Mostrando dispositivos Bluetooth.", Toast.LENGTH_SHORT).show();

                    adapter = new ArrayAdapter(MainActivity.this, android.R.layout.simple_list_item_1, list);

                    deviceList.setAdapter(adapter);
                }

                private void turnOffBluetooth() {
                    if (bluetoothAdapter != null) {
                        deviceList.setAdapter(null);
                        bluetoothAdapter.disable();
                        Toast.makeText(getApplicationContext(), "Apagando Bluetooth.", Toast.LENGTH_SHORT).show();
                    }
                }
            });
        }
    }

    private void validateBluetoothState() {
        if (bluetoothAdapter.isEnabled()) {
            this.list();
            bluetoothSwitch.setChecked(true);
        }
    }

    public void openAboutUs(View view) {
        Intent intent = new Intent(this, AboutUs.class);
        startActivity(intent);
    }

    public void openImageSlideshow(View view) {
        Intent intent = new Intent(this, ImageSlideshow.class);
        startActivity(intent);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        try {
            super.onActivityResult(requestCode, resultCode, data);
            if (requestCode == REQUEST_CODE  && resultCode  == RESULT_OK) {
                this.list();
            }
        } catch (Exception ex) {
            Toast.makeText(MainActivity.this, "Ocurrio un error",
                    Toast.LENGTH_SHORT).show();
        }

    }

    public void list() {
        pairedDevices = bluetoothAdapter.getBondedDevices();
        deviceList = (ListView) findViewById(R.id.listView);
        ArrayList list = new ArrayList();

        for (BluetoothDevice bt : pairedDevices) {
            list.add(bt.getName());
        }
        Toast.makeText(getApplicationContext(), "Mostrando dispositivos Bluetooth.", Toast.LENGTH_SHORT).show();

        adapter = new ArrayAdapter(MainActivity.this, android.R.layout.simple_list_item_1, list);

        deviceList.setAdapter(adapter);
    }

}

