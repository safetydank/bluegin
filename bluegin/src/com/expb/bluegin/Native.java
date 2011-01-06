package com.expb.bluegin;

public class Native
{
    //  BlueGin native entry points
    public static native void create();
    public static native void setup();
    public static native void update();
    public static native void draw();
    public static native void cleanup();
    public static native void pause();

    public static native void setScreenSize(int width, int height);
    public static native void addTouchEvent(int eventType, float x, float y, float px, float py, int id);
    public static native void setTouches();
    public static native void setAccelerometer(float x, float y, float z);
}

