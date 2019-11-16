package com.unlam.hologramhand;

import android.content.Context;
import android.widget.Toast;

public class ToasterPrinter {

    public static void printToasterLong(String message, Context context){
        Toast.makeText(context.getApplicationContext(), message, Toast.LENGTH_LONG).show();
    }

    public static void printToasterShort(String message, Context context){
        Toast.makeText(context.getApplicationContext(), message, Toast.LENGTH_SHORT).show();
    }
}
