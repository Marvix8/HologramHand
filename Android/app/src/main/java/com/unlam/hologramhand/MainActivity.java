package com.unlam.hologramhand;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
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
    private Set<String> devices;
    private ListView deviceList;
    private ArrayAdapter adapter;
    private BluetoothAdapter bluetoothAdapter;
    private BroadcastReceiver broadcastReceiver;
    public static final int REQUEST_CODE = 1;
    public static final int ACTIVATE_DISCOVER_BLUETOOTH = 2;

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
                        startActivityForResult(turnOn, 0);
                        Toast.makeText(getApplicationContext(), "Activando",Toast.LENGTH_SHORT).show();
                    }
                }

                private void turnOffBluetooth() {
                    if (bluetoothAdapter != null) {
                        if(bluetoothAdapter.isDiscovering()){
                            bluetoothAdapter.cancelDiscovery();
                        }
                        if(deviceList != null) {
                            deviceList.setAdapter(null);
                        }
                        bluetoothAdapter.disable();
                        Toast.makeText(getApplicationContext(), "Apagando Bluetooth.", Toast.LENGTH_SHORT).show();
                    }
                }
            });
        }
    }

    private void validateBluetoothState() {
        if (bluetoothAdapter != null && bluetoothAdapter.isEnabled()) {
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

    public void searchDevices(View view){
        if(bluetoothAdapter == null){
            Toast.makeText(getApplicationContext(), "Error al buscar dispositivos.", Toast.LENGTH_SHORT).show();
        }else if(bluetoothAdapter != null && !bluetoothAdapter.isEnabled()) {
            Toast.makeText(getApplicationContext(), "Encienda el bluetooth.", Toast.LENGTH_SHORT).show();
        }else {
                IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
                if (!bluetoothAdapter.isDiscovering()) {
                    if(adapter != null){
                        adapter.clear();
                    }
                    MainActivity.this.registerReceiver(bReciever, filter);
                    bluetoothAdapter.startDiscovery();
                } else {
                    MainActivity.this.unregisterReceiver(bReciever);
                    bluetoothAdapter.cancelDiscovery();
                }
            }
    }

    private final BroadcastReceiver bReciever = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                System.out.println(device.getName());
                System.out.println("--------------------------------");
                // Create a new device item
                DeviceItem newDevice = new DeviceItem(device.getName(), device.getAddress(), "false");
                // Add it to our adapter
                adapter.add(newDevice);
            }
        }
    };


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        try {
            super.onActivityResult(requestCode, resultCode, data);
            if (resultCode  == RESULT_OK) {
                if(requestCode == ACTIVATE_DISCOVER_BLUETOOTH){
                    System.out.println(data);
                }
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

//    public void list() {
//        pairedDevices = bluetoothAdapter.getBondedDevices();
//        deviceList = (ListView) findViewById(R.id.listView);
//        ArrayList list = new ArrayList();
//
//        for (BluetoothDevice bt : pairedDevices) {
//            list.add(bt.getName());
//        }
//        Toast.makeText(getApplicationContext(), "Mostrando dispositivos Bluetooth.", Toast.LENGTH_SHORT).show();
//
//        adapter = new ArrayAdapter(MainActivity.this, android.R.layout.simple_list_item_1, list);
//
//        deviceList.setAdapter(adapter);
//    }


}

