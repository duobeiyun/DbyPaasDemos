package com.duobeiyun.multi_sdk_demo.utils;

import android.content.Context;
import android.content.SharedPreferences;

import static android.content.Context.MODE_PRIVATE;


public class SharePreUtils {

    private static SharePreUtils instance;
    private SharedPreferences preferences;
    private SharedPreferences.Editor editor;

    /**
     * Construtor
     *
     * @param context
     */
    private SharePreUtils(Context context) {
        preferences = context.getSharedPreferences("dby_paas_sdk", MODE_PRIVATE);
    }

    /**
     * Metodo que retorna a instancia da classe
     *
     * @param context
     * @return
     */
    public static synchronized SharePreUtils getInstance(Context context) {
        if (SharePreUtils.instance == null) {
            SharePreUtils.instance = new SharePreUtils(context);
        }
        return SharePreUtils.instance;
    }

    /**
     * Retorna valor String da map
     *
     * @param key
     * @param returnOnNull
     * @return
     */
    public String getValue(String key, String returnOnNull) {
        return preferences.getString(key, returnOnNull);
    }

    /**
     * Retorna valor booleano da map
     *
     * @param key
     * @param returnOnNull
     * @return
     */
    public boolean getValue(String key, boolean returnOnNull) {
        return preferences.getBoolean(key, returnOnNull);
    }

    /**
     * Retorna valor int da map
     *
     * @param key
     * @param returnOnNull
     * @return
     */
    public int getValue(String key, int returnOnNull) {
        return preferences.getInt(key, returnOnNull);
    }
    public long getValue(String key, long returnOnNull) {
        return preferences.getLong(key, returnOnNull);
    }


    /**
     * Seta valor String na map
     *
     * @param key
     * @param value
     */
    public void setValue(String key, String value) {
        editor = preferences.edit();
        editor.putString(key, value);
        editor.commit();
    }

    /**
     * Seta valor booleano na map
     *
     * @param key
     * @param value
     */
    public void setValue(String key, boolean value) {
        editor = preferences.edit();
        editor.putBoolean(key, value);
        editor.commit();
    }

    /**
     * Seta valor int na map
     *
     * @param key
     * @param value
     */
    public void setValue(String key, int value) {
        editor = preferences.edit();
        editor.putInt(key, value);
        editor.commit();
    }
    public void setValue(String key, long value) {
        editor = preferences.edit();
        editor.putLong(key, value);
        editor.commit();
    }
    /**
     * Remove uma determinada chave da map
     *
     * @param key
     */
    public void remove(String key) {
        editor = preferences.edit();
        editor.remove(key);
        editor.commit();
    }
}
