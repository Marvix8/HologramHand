package com.unlam.hologramhand;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.util.Log;

import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class BluetoothService extends Thread {
    private static BluetoothService bluetoothService;

    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private final Context context;
    private final String conectionFailure;
    private final int minusOne;
    private final String socket;
    private final String failDevice;
    private final String conected;
    private final String failed;
    private BluetoothAdapter btAdapter;
    private BluetoothDevice btDevice;
    private BluetoothSocket btSocket;
    private InputStream mmInStream;
    private OutputStream mmOutStream;
    private StringBuilder recDataString;
    private Handler handlerBluetoothIn;
    private int handlerState = 0;

    private BluetoothService(Context context, String deviceAddress, final String item) {

        this.context = context;

        this.minusOne = Integer.valueOf(this.context.getString(R.string.minus_one));
        this.conectionFailure = this.context.getString(R.string.conection_failure);
        this.socket = this.context.getString(R.string.socket);
        this.conected = this.context.getString(R.string.conected);
        this.failDevice = this.context.getString(R.string.fail_device);
        this.failed = this.context.getString(R.string.failed);

        this.btAdapter = BluetoothAdapter.getDefaultAdapter();

        this.btDevice = btAdapter.getRemoteDevice(deviceAddress);

        try {
            this.btSocket = btDevice.createRfcommSocketToServiceRecord(BTMODULEUUID);
        } catch (IOException e) {
            Log.i(this.socket, this.failDevice + item);
        }
        try {
            this.btSocket.connect();
            Log.i(this.socket, this.conected + item);
        } catch (IOException e) {
            Log.i(this.socket, this.failed + item + " " + e.toString());
        }

        InputStream tmpIn = null;
        OutputStream tmpOut = null;

        try {
            tmpIn = this.btSocket.getInputStream();
            tmpOut = this.btSocket.getOutputStream();
        } catch (IOException e) {
        }

        this.mmInStream = tmpIn;
        this.mmOutStream = tmpOut;
        this.recDataString = new StringBuilder();
    }

    public static BluetoothService create(Context context, String deviceAddress, final String item) {
        if (bluetoothService == null) {
            bluetoothService = new BluetoothService(context, deviceAddress, item);
        }
        return bluetoothService;
    }

    public static void destroyBluetooth() {
        bluetoothService = null;
    }

    public static BluetoothService create() {
        return bluetoothService;
    }

    public void setHandler(Handler handlerBluetoothIn) {
        this.handlerBluetoothIn = handlerBluetoothIn;
    }

    //metodo run del hilo, que va a entrar en una espunregisterReceiverera activa para recibir los msjs del HC05
    public void run() {
        final int bufferSize = 256;
        final int zero = 0;

        byte[] buffer = new byte[bufferSize];
        int bytes;

        //el hilo secundario se queda esperando mensajes del HC05
        while (true) {
            try {
                //se leen los datos del Bluethoot
                bytes = this.mmInStream.read(buffer);
                String readMessage = new String(buffer, zero, bytes);
                System.out.println(readMessage);
                //se muestran en el layout de la activity, utilizando el handler del hilo
                // principal antes mencionado

                this.handlerBluetoothIn.obtainMessage(this.handlerState, bytes, this.minusOne, readMessage).sendToTarget();
            } catch (IOException e) {
                break;
            }
        }
    }

    //write method
    public void write(String input) {
        byte[] msgBuffer = input.getBytes();           //converts entered String into bytes
        try {
            this.mmOutStream.write(msgBuffer);                //write bytes over BT connection via outstream
        } catch (IOException e) {
            //if you cannot write, close the application
            Log.i(this.conectionFailure, e.toString());
        }
    }

    //Handler que sirve que permite mostrar datos en el Layout al hilo secundario
    public Handler HandlerMensajeHiloPrincipal(final Context mainContext) {
        final LocalBroadcastManager localBroadcastManager = LocalBroadcastManager.getInstance(mainContext);

        return new Handler() {

            public void handleMessage(android.os.Message msg) {
                //si se recibio un msj del hilo secundario
                if (msg.what == handlerState) {
                    //voy concatenando el msj
                    String readMessage = (String) msg.obj;
                    recDataString = new StringBuilder(); //TODO: eliminar

                    //cuando recibo toda una linea la muestro en el layout
                    if (readMessage.length() > Integer.valueOf(mainContext.getString(R.string.zero))) {
                        Intent intent = new Intent(mainContext.getString(R.string.gesture_instruction));
                        intent.putExtra(mainContext.getString(R.string.instruction), readMessage);
                        localBroadcastManager.sendBroadcast(intent);

                        recDataString.delete(Integer.valueOf(mainContext.getString(R.string.zero)), recDataString.length());
                    }
                }
            }
        };

    }

}