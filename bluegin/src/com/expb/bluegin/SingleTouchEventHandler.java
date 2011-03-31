package com.expb.bluegin;

import android.view.MotionEvent;
import com.expb.bluegin.BlueGinInput;

//  Handles Touch events for versions of Android earlier than 2.2 (no multi-touch)
public class SingleTouchEventHandler extends TouchEventHandler {
    BlueGinInput mInput = null;
    float prevX;
    float prevY;

    public SingleTouchEventHandler(BlueGinInput input)
    {
        mInput = input;
        prevX = prevY = 0;
    }

    public boolean onTouchEvent(MotionEvent ev)
    {
        final int action = ev.getAction();
        final float x = ev.getX();
        final float y = ev.getY();

        switch (action) {
            //  Touch began
            case MotionEvent.ACTION_DOWN:
                mInput.addTouchEvent(0, x, y, x, y, 0);
                prevX = x;
                prevY = y;
                break;

            //  Touch moved
            case MotionEvent.ACTION_MOVE:
                mInput.addTouchEvent(1, x, y, prevX, prevY, 0);
                prevX = x;
                prevY = y;
                break;

            //  Touch ended
            case MotionEvent.ACTION_UP:
                mInput.addTouchEvent(2, x, y, prevX, prevY, 0);
                break;
        }
        return true;
    }
}


