//  A class to handle accelerometer input
//
//
package com.expb.bluegin;

import android.content.Context;
import android.hardware.SensorManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;

public class Accelerometer implements SensorEventListener
{
    private Sensor mAccel = null;
    private SensorManager mManager = null;

    private float[] mValues = { 0, 0, 0 };

    public boolean updated = false;
    public float[] getValues() { updated = false; return mValues; }

    public Accelerometer(Context context)
    {
        mManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
        mAccel = mManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
    }

    public boolean available()
    {
        return (mAccel != null);
    }

    public void enable()
    {
        if (available()) {
            mManager.registerListener(this, mAccel, SensorManager.SENSOR_DELAY_GAME); 
        }
    }

    public void disable()
    {
        mManager.unregisterListener(this);
    }

    public void onAccuracyChanged(Sensor sensor, int accuracy)
    {
    }

    public void onSensorChanged(SensorEvent event)
    {
        float[] sensed = event.values;
        for (int i=0; i < sensed.length && i < 3; ++i) {
            mValues[i] = sensed[i];
            updated = true;
        }
    }
}
