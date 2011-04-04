package com.expb.bluegin;

import android.content.Context;

import com.expb.bluegin.Accelerometer;
import java.util.ArrayList;
import java.util.Iterator;

public class BlueGinInput
{
    public class Touch
    {
        public int eventType;
        public float x;
        public float y;
        public float px;
        public float py;
        public int id;

        public Touch(int eventType, float x, float y, float px, float py, int id)
        {
            this.eventType = eventType;
            this.x = x;
            this.y = y;
            this.px = px;
            this.py = py;
            this.id = id;
        }
    }

    public class Key
    {
        public boolean keyDown;
        public int unicode; 
        public long modifiers;
        public long keycode;

        public Key(boolean keyDown, int unicode, long modifiers, long keycode) 
        {
            this.keyDown = keyDown;
            this.unicode = unicode;
            this.modifiers = modifiers;
            this.keycode = keycode;
        }
    }

    private ArrayList mTouches = null;
    private ArrayList mKeys    = null;
    private Accelerometer mAccel = null;
    public  Accelerometer getAccelerometer() { return mAccel; }

    //  Cinder key modifiers
	private static int SHIFT_DOWN = 0x0008;
	private static int ALT_DOWN	  = 0x0010;
	private static int CTRL_DOWN  = 0x0020;
	private static int META_DOWN  = 0x0040;
	private static int ACCEL_DOWN = META_DOWN;

    public BlueGinInput(Context context)
    {
        mTouches = new ArrayList();
        mKeys = new ArrayList();
        mAccel = new Accelerometer(context);
        mAccel.enable();
    }

    public void stop()
    {
        mAccel.disable();
        mAccel = null;
    }
    
    public void addTouchEvent(int eventType, float x, float y, float px, float py, int id)
    {
        synchronized (this) {
            mTouches.add(new Touch(eventType, x, y, px, py, id));
        }
    }

    public void addKeyEvent(boolean keyDown, int unicode, long modifiers, long nativeKeyCode) 
    {
        synchronized (this) {
            mKeys.add(new Key(keyDown, unicode, modifiers, nativeKeyCode));
        }
    }

    public void nativeUpdate()
    {
        synchronized (this) {
            Iterator it = mTouches.iterator();
            while (it.hasNext()) {
                Touch touch = (Touch) it.next();
                Native.addTouchEvent(touch.eventType, touch.x, touch.y, touch.px, touch.py, touch.id);
            }

            it = mKeys.iterator();
            while (it.hasNext()) {
                Key key = (Key) it.next();
                Native.addKeyEvent(key.keyDown, key.unicode, (int) key.modifiers, (int) key.keycode);
            }
        }

        Native.setTouches();
        Native.setKeys();

        mTouches.clear();
        mKeys.clear();

        float[] acc = mAccel.getValues();
        Native.setAccelerometer(acc[0], acc[1], acc[2]);
    }
};
