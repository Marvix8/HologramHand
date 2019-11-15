package com.unlam.hologramhand;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.CompoundButton;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.Switch;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private Switch bluetoothSwitch;
    private ListView deviceList;
    private ArrayAdapter adapter;
    private BluetoothAdapter bluetoothAdapter;
    public BluetoothService bluetoothService;
    public static final int REQUEST_CODE = 1;
    public static final int ACTIVATE_DISCOVER_BLUETOOTH = 2;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        adapter = new ArrayAdapter(MainActivity.this, android.R.layout.simple_list_item_activated_1);
        setContentView(R.layout.activity_main);
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        bluetoothSwitch = (Switch) findViewById(R.id.bluetoothSwitch);
        validateBluetoothState();
        if (ContextCompat.checkSelfPermission(MainActivity.this,
                Manifest.permission.ACCESS_COARSE_LOCATION)
                != PackageManager.PERMISSION_GRANTED) {

            ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.ACCESS_COARSE_LOCATION},
                    REQUEST_CODE);
        }
        if (bluetoothSwitch != null) {
            bluetoothSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                    if (isChecked) {
                        this.turnOnBluetooth();
                    } else {
                        this.turnOffBluetooth();
                    }
                }

                public void turnOnBluetooth() {
                    System.out.println(bluetoothAdapter.isEnabled());
                    if (!bluetoothAdapter.isEnabled()) {
                        Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                        startActivityForResult(turnOn, 0);
                        Toast.makeText(getApplicationContext(), "Activando", Toast.LENGTH_SHORT).show();
                    }
                }

                private void turnOffBluetooth() {
                    if (bluetoothAdapter != null) {
                        if (bluetoothAdapter.isDiscovering()) {
                            bluetoothAdapter.cancelDiscovery();
                        }
                        if (deviceList != null) {
                            deviceList.setAdapter(null);
                        }
                        bluetoothAdapter.disable();
                        Toast.makeText(getApplicationContext(), "Apagando Bluetooth.", Toast.LENGTH_SHORT).show();
                    }
                }
            });
        }

        if (deviceList == null) {
            deviceList = (ListView) findViewById(R.id.listView);
        }

        deviceList.setOnItemClickListener(new AdapterView.OnItemClickListener()
        {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {

                Toast.makeText(MainActivity.this, adapter.getItem(position).toString(), Toast.LENGTH_LONG).show();
                String item = adapter.getItem(position).toString();
                String[] stringItemArray = item.split("º ");
                MainActivity.this.bluetoothService = new BluetoothService(stringItemArray[1], stringItemArray[0]);
                MainActivity.this.bluetoothService.setHandler(MainActivity.this.bluetoothService.HandlerMensajeHiloPrincipal(MainActivity.this));
                MainActivity.this.bluetoothService.start();

            }
        });
    }

    private void printToaster(String message){
        Toast.makeText(getApplicationContext(), message, Toast.LENGTH_LONG).show();
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
        Intent intent = new Intent(this, VideoPlayer.class);
        startActivity(intent);
    }


    public void searchDevices(View view) {
        ImageButton btn = (ImageButton)findViewById(R.id.searchDevices);

        if (bluetoothAdapter == null) {
            Toast.makeText(getApplicationContext(), "Error al buscar dispositivos.", Toast.LENGTH_SHORT).show();
        } else if (bluetoothAdapter != null && !bluetoothAdapter.isEnabled()) {
            Toast.makeText(getApplicationContext(), "Encienda el bluetooth.", Toast.LENGTH_SHORT).show();
        } else {
            IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
            if (!bluetoothAdapter.isDiscovering()) {
                if (adapter != null) {
                    adapter.clear();
                }
                MainActivity.this.registerReceiver(bReciever, filter);
                bluetoothAdapter.startDiscovery();
                btn.setImageResource(R.drawable.ic_cancel_black_24dp);
            } else {
                MainActivity.this.unregisterReceiver(bReciever);
                bluetoothAdapter.cancelDiscovery();
                btn.setImageResource(R.drawable.ic_search_black_24dp);
            }
        }
    }

    private final BroadcastReceiver bReciever = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                DeviceItem newDevice = new DeviceItem(device.getName(), device.getAddress(), "false");
                adapter.add(newDevice.toString());
                if (deviceList == null) {
                    deviceList = (ListView) findViewById(R.id.listView);
                }
                deviceList.setAdapter(adapter);
            }
        }
    };

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           String permissions[], int[] grantResults) {
        switch (requestCode) {
            case REQUEST_CODE: {
                System.out.println("permission granted");

            }
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        try {
            super.onActivityResult(requestCode, resultCode, data);
            if (resultCode == RESULT_OK) {
                if (requestCode == ACTIVATE_DISCOVER_BLUETOOTH) {
                    System.out.println(data);
                }
            }
        } catch (Exception ex) {
            Toast.makeText(MainActivity.this, "Ocurrio un error",
                    Toast.LENGTH_SHORT).show();
        }

    }
}