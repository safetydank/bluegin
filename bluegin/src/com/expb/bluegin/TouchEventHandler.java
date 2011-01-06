package com.expb.bluegin;

import android.os.Build;
import android.view.MotionEvent;

import com.expb.bluegin.BlueGinInput;

import com.expb.bluegin.MultiTouchEventHandler;
import com.expb.bluegin.SingleTouchEventHandler;

public abstract class TouchEventHandler 
{
    public abstract boolean onTouchEvent(MotionEvent ev);

    //  Creates an appropriate touch handler depending on the OS version
    public static TouchEventHandler create(BlueGinInput input) 
    {
        final int sdkVersion = Integer.parseInt(Build.VERSION.SDK);

        TouchEventHandler handler;

        if (sdkVersion < Build.VERSION_CODES.FROYO) {
            handler = new SingleTouchEventHandler(input);
        } else {
            handler = new MultiTouchEventHandler(input);
        }

        return handler;
    }
}

