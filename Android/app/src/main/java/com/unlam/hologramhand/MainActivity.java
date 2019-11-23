package com.unlam.hologramhand;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.CompoundButton;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.Switch;
import android.widget.Toast;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.preference.PreferenceFragmentCompat;

public class MainActivity extends AppCompatActivity implements SensorEventListener {

    private static final float SHAKE_THRESHOLD = 800;
    private Switch bluetoothSwitch;
    private ListView deviceList;
    private ArrayAdapter adapter;
    private BluetoothAdapter bluetoothAdapter;
    public BluetoothService bluetoothService;
    public static final int REQUEST_CODE = 1;
    public static final int ACTIVATE_DISCOVER_BLUETOOTH = 2;
    private final BroadcastReceiver bReciever = this.createBroadcastReceiver();
    private SensorManager sensorManager;
    private Sensor accelerometer;
    private long lastUpdate;
    private float x;
    private float y;
    private float z;
    private float last_x;
    private float last_y;
    private float last_z;
    private int shakeUpdateTime;
    private int tenSeconds;
    private int axisX;
    private int axisY;
    private int axisZ;
    private String anErrorHasOccurred;
    private String permissionGranted;
    private String turnOnBluetoothMessage;
    private String errorFindingDevices;
    private String turningOffBluetooth;
    private String turningOnBluetooth;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        this.shakeUpdateTime = Integer.valueOf(getString(R.string.shake_update_time));
        this.tenSeconds = Integer.valueOf(getString(R.string.ten_seconds));
        this.axisX = Integer.valueOf(getString(R.string.axis_x));
        this.axisY = Integer.valueOf(getString(R.string.axis_y));
        this.axisZ = Integer.valueOf(getString(R.string.axis_z));
        this.anErrorHasOccurred = getString(R.string.an_error_has_occurred);
        this.permissionGranted = getString(R.string.permission_granted);
        this.turnOnBluetoothMessage = getString(R.string.turn_on_bluetooth_message);
        this.errorFindingDevices = getString(R.string.error_finding_devices);
        this.turningOffBluetooth = getString(R.string.turning_off_bluetooth);
        this.turningOnBluetooth = getString(R.string.turning_on_bluetooth);


        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        adapter = new ArrayAdapter(MainActivity.this, android.R.layout.simple_list_item_activated_1);
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        bluetoothSwitch = (Switch) findViewById(R.id.bluetoothSwitch);

        this.setStateBluetoothSwitch(isBluetoothEnabled());

        this.initializeSensors();

        if (ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, REQUEST_CODE);
        }

        if (bluetoothSwitch != null) {
            bluetoothSwitch.setOnCheckedChangeListener(this.createBluetoothListener());
        }

        if (deviceList == null) {
            deviceList = (ListView) findViewById(R.id.listView);
        }

        deviceList.setOnItemClickListener(this.getOnClickListenerBluetoothList());
        getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.action_settings:
                Intent intent = new Intent(this, SettingsActivity.class);
                startActivity(intent);
                break;
            default:
                break;
        }
        return true;
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
                BluetoothService.destroyBluetooth();
                MainActivity.this.bluetoothService = BluetoothService.create(MainActivity.this, stringItemArray[1], stringItemArray[0]);
                MainActivity.this.bluetoothService.setHandler(MainActivity.this.bluetoothService.HandlerMensajeHiloPrincipal(MainActivity.this));
                MainActivity.this.bluetoothService.start();
            }
        };
    }

    private void turnOnBluetooth() {
        if (!bluetoothAdapter.isEnabled()) {
            Intent turnOn = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnOn, 0);
            ToasterPrinter.printToasterShort(turningOnBluetooth, this);
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
            ToasterPrinter.printToasterShort(turningOffBluetooth, this);
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
        return this.existBluetoothAdapter() && this.isBluetoothAdapterEnabled();
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
            ToasterPrinter.printToasterShort(errorFindingDevices, this);
        } else if (this.existBluetoothAdapter() && !this.isBluetoothAdapterEnabled()) {
            ToasterPrinter.printToasterShort(turnOnBluetoothMessage, this);
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
                System.out.println(permissionGranted);
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
            ToasterPrinter.printToasterShort(anErrorHasOccurred, this);
        }
    }


    private void initializeSensors() {
        this.sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        this.accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);

        this.sensorManager.registerListener(this, this.accelerometer, SensorManager.SENSOR_DELAY_GAME);

        this.InitializeAccelerometerVariables();
    }

    private void InitializeAccelerometerVariables() {
        lastUpdate = Integer.valueOf(this.getString(R.string.zero));
        x = Integer.valueOf(this.getString(R.string.zero));
        y = Integer.valueOf(this.getString(R.string.zero));
        z = Integer.valueOf(this.getString(R.string.zero));
        last_x = Integer.valueOf(this.getString(R.string.zero));
        last_y = Integer.valueOf(this.getString(R.string.zero));
        last_z = Integer.valueOf(this.getString(R.string.zero));
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            long curTime = System.currentTimeMillis();
            // only allow one update every 100ms.
            if ((curTime - lastUpdate) > this.shakeUpdateTime) {
                long diffTime = (curTime - lastUpdate);
                lastUpdate = curTime;

                x = event.values[axisX];
                y = event.values[axisY];
                z = event.values[axisZ];

                float speed = Math.abs(x + y + z - last_x - last_y - last_z) / diffTime * tenSeconds;

                if (speed > SHAKE_THRESHOLD && this.bluetoothService != null) {
                    this.bluetoothService.write("L");
                }
                last_x = x;
                last_y = y;
                last_z = z;
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }
}