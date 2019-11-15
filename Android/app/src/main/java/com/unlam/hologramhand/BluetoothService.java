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

    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private BluetoothAdapter btAdapter;
    private BluetoothDevice btDevice;
    private BluetoothSocket btSocket;
    private InputStream mmInStream;
    private OutputStream mmOutStream;
    private StringBuilder recDataString;
    private Handler handlerBluetoothIn;
    private int handlerState = 0;
    private String item;
    private boolean finalizado = false;

    public BluetoothService(String deviceAddress, final String item) {
        this.item = item;

        btAdapter = BluetoothAdapter.getDefaultAdapter();

        btDevice = btAdapter.getRemoteDevice(deviceAddress);
        try {
            btSocket = btDevice.createRfcommSocketToServiceRecord(BTMODULEUUID);
        } catch (IOException e) {
            Log.i("SOCKET", "fallo dispositivo " + item);
        }
        try {
            btSocket.connect();
            Log.i("SOCKET", "conectado " + item);
        } catch (IOException e) {
            Log.i("SOCKET", "fallo " + item + " " + e.toString());
        }

        InputStream tmpIn = null;
        OutputStream tmpOut = null;

        try {
            tmpIn = btSocket.getInputStream();
            tmpOut = btSocket.getOutputStream();
        } catch (IOException e) {
        }

        mmInStream = tmpIn;
        mmOutStream = tmpOut;
        this.recDataString  = new StringBuilder();
    }

    public void setHandler(Handler handlerBluetoothIn) {
        this.handlerBluetoothIn = handlerBluetoothIn;
    }

    public void setItem(String item) {
        this.item = item;
    }

    //metodo run del hilo, que va a entrar en una espunregisterReceiverera activa para recibir los msjs del HC05
    public void run()
    {
        byte[] buffer = new byte[256];
        int bytes;

        //el hilo secundario se queda esperando mensajes del HC05
        while (true)
        {
            try
            {
                //se leen los datos del Bluethoot
                bytes = mmInStream.read(buffer);
                String readMessage = new String(buffer, 0, bytes);
                System.out.println(readMessage);
                //se muestran en el layout de la activity, utilizando el handler del hilo
                // principal antes mencionado

                handlerBluetoothIn.obtainMessage(handlerState, bytes, -1, readMessage).sendToTarget();
            } catch (IOException e) {
                break;
            }
        }
    }

    //write method
    public void write(String input) {
        byte[] msgBuffer = input.getBytes();           //converts entered String into bytes
        try {
            mmOutStream.write(msgBuffer);                //write bytes over BT connection via outstream
        } catch (IOException e) {
            //if you cannot write, close the application
            Log.i("La conexion fallo", e.toString());
        }
    }

    //Handler que sirve que permite mostrar datos en el Layout al hilo secundario
    public Handler HandlerMensajeHiloPrincipal(Context mainContext)
    {
        final LocalBroadcastManager localBroadcastManager = LocalBroadcastManager.getInstance(mainContext);

        return new Handler() {

            public void handleMessage(android.os.Message msg)
            {
                //si se recibio un msj del hilo secundario
                if (msg.what == handlerState)
                {
                    //voy concatenando el msj
                    String readMessage = (String) msg.obj;
                    recDataString.append(readMessage);
                    int endOfLineIndex = recDataString.indexOf("\r\n");

                    //cuando recibo toda una linea la muestro en el layout
                    if (endOfLineIndex > 0)
                    {
                        String instruction = recDataString.substring(0, endOfLineIndex);
                        System.out.println("---->" + instruction);

                        Intent intent = new Intent("gesture-instruction");
                        intent.putExtra("instruction", instruction);
                        localBroadcastManager.sendBroadcast(intent);

                        recDataString.delete(0, recDataString.length());
                    }
                }
            }
        };

    }

}