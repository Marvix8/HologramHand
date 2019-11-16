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

public class MainActivity extends AppCompatActivity {

    private Switch bluetoothSwitch;
    private ListView deviceList;
    private ArrayAdapter adapter;
    private BluetoothAdapter bluetoothAdapter;
    public BluetoothService bluetoothService;
    public static final int REQUEST_CODE = 1;
    public static final int ACTIVATE_DISCOVER_BLUETOOTH = 2;
    private final BroadcastReceiver bReciever = this.createBroadcastReceiver();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        adapter = new ArrayAdapter(MainActivity.this, android.R.layout.simple_list_item_activated_1);
        setContentView(R.layout.activity_main);
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        bluetoothSwitch = (Switch) findViewById(R.id.bluetoothSwitch);

        this.setStateBluetoothSwitch(isBluetoothEnabled());

        if (ContextCompat.checkSelfPermission(MainActivity.this,Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.ACCESS_COARSE_LOCATION},REQUEST_CODE);
        }

        if (bluetoothSwitch != null) {
            bluetoothSwitch.setOnCheckedChangeListener(this.createBluetoothListener());
        }

        if (deviceList == null) {
            deviceList = (ListView) findViewById(R.id.listView);
        }

        deviceList.setOnItemClickListener(this.getOnClickListenerBluetoothList());
    }

    public void openVideoPlayer(View view) {
        Intent intent = new Intent(this, VideoPlayer.class);
        startActivity(intent);
    }

    public void openAboutUs(View view) {
        Intent intent = new Intent(this, AboutUs.class);
        startActivity(intent);
    }

    private AdapterView.OnItemClickListener getOnClickListenerBluetoothList() {
        return new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                ToasterPrinter.printToasterLong(adapter.getItem(position).toString(), MainActivity.this);
                String item = adapter.getItem(position).toString();
                String[] stringItemArray = item.split("º ");
                MainActivity.this.bluetoothService = new BluetoothService(MainActivity.this, stringItemArray[1], stringItemArray[0]);
                MainActivity.this.bluetoothService.setHandler(MainActivity.this.bluetoothService.HandlerMensajeHiloPrincipal(MainActivity.this));
                MainActivity.this.bluetoothService.start();
            }
        };
    }

    private void turnOnBluetooth() {
        if (!bluetoothAdapter.isEnabled()) {
            Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnOn, 0);
            ToasterPrinter.printToasterShort("Activando.", this);
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
            ToasterPrinter.printToasterShort("Apagando Bluetooth.", this);
        }
    }

    private CompoundButton.OnCheckedChangeListener createBluetoothListener() {
        return new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    MainActivity.this.turnOnBluetooth();
                } else {
                    MainActivity.this.turnOffBluetooth();
                }
            }
        };
    }

    private boolean isBluetoothEnabled() {
        return bluetoothAdapter != null && bluetoothAdapter.isEnabled();
    }

    private void setStateBluetoothSwitch(boolean state) {
        this.bluetoothSwitch.setChecked(state);
    }


    private boolean existBluetoothAdapter() {
        return this.bluetoothAdapter != null;
    }

    private boolean isBluetoothAdapterEnabled() {
        return bluetoothAdapter.isEnabled();
    }

    public void searchDevices(View view) {
        ImageButton bluetoothButton = (ImageButton) findViewById(R.id.searchDevices);

        if (!this.existBluetoothAdapter()) {
            ToasterPrinter.printToasterShort("Error al buscar dispositivos.", this);
        } else if (this.existBluetoothAdapter() && !this.isBluetoothAdapterEnabled()) {
            ToasterPrinter.printToasterShort("Encienda el bluetooth.", this);
        } else {
            IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
            if (!bluetoothAdapter.isDiscovering()) {
                if (adapter != null) {
                    adapter.clear();
                }
                bluetoothButton.setImageResource(this.connectBluetooth(filter));
            } else {
                bluetoothButton.setImageResource(this.disconnectBluetooth());
            }
        }
    }

    private int disconnectBluetooth() {
        MainActivity.this.unregisterReceiver(bReciever);
        bluetoothAdapter.cancelDiscovery();
        return R.drawable.ic_search_black_24dp;
    }

    private int connectBluetooth(IntentFilter filter) {
        MainActivity.this.registerReceiver(bReciever, filter);
        bluetoothAdapter.startDiscovery();
        return R.drawable.ic_cancel_black_24dp;
    }

    private BroadcastReceiver createBroadcastReceiver() {
     return new BroadcastReceiver() {
            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                    BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                    DeviceItem newDevice = new DeviceItem(device.getName(), device.getAddress());
                    adapter.add(newDevice.toString());
                    if (deviceList == null) {
                        deviceList = (ListView) findViewById(R.id.listView);
                    }
                    deviceList.setAdapter(adapter);
                }
            }
        };
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
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
            ToasterPrinter.printToasterShort("Ocurrio un error", this);
        }
    }
}