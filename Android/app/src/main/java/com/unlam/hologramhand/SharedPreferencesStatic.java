package com.unlam.hologramhand;

import android.content.Context;
import android.content.SharedPreferences;

import static android.content.Context.MODE_PRIVATE;

public class SharedPreferencesStatic {

    public static boolean existPreference(Context context, String sharedPreference, String preference){
        SharedPreferences sharedPreferences = context.getApplicationContext().getSharedPreferences(sharedPreference, MODE_PRIVATE);
        int value = sharedPreferences.getInt(preference, -1);
        return value != -1;
    }

    public static int getIntPreference(Context context, String sharedPreference,String preference){
        SharedPreferences sharedPreferences = context.getApplicationContext().getSharedPreferences(sharedPreference, MODE_PRIVATE);
        return sharedPreferences.getInt(preference, -1);
    }
}
