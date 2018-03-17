package com.example.panyi.mosaicdemo;

import android.graphics.Bitmap;

/**
 * Created by panyi on 18-3-17.
 */
public final class NativeProcess {
    static {
        System.loadLibrary("native-lib");
    }

    public static native String stringFromJNI();

    public static void mosaticFilter(Bitmap src , Bitmap out , int radius){
        if(src==null || src.isRecycled() || out== null || out.isRecycled())
            return;
        if(radius<=1)
            return;

        mosaic(src , out , radius);
    }

    public static native void mosaic(Bitmap srcBitmap,Bitmap outBitmap ,int radius);
}
