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

    private ArrayList mTouches = null;
    private Accelerometer mAccel = null;
    public Accelerometer getAccelerometer() { return mAccel; }

    public BlueGinInput(Context context)
    {
        mTouches = new ArrayList();
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

    public void nativeUpdate()
    {
        synchronized (this) {
            Iterator it = mTouches.iterator();
            while (it.hasNext()) {
                Touch touch = (Touch) it.next();
                Native.addTouchEvent(touch.eventType, touch.x, touch.y, touch.px, touch.py, touch.id);
            }
        }
        Native.setTouches();
        mTouches.clear();

        float[] acc = mAccel.getValues();
        Native.setAccelerometer(acc[0], acc[1], acc[2]);
    }
};
