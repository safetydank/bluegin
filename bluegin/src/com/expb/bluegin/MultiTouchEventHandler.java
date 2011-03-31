package com.expb.bluegin;

import android.view.MotionEvent;
import com.expb.bluegin.BlueGinInput;

public class MultiTouchEventHandler extends TouchEventHandler {
    BlueGinInput mInput = null;

    //  Maximum number of points tracked
    static final int MAX_POINTS = 10;
    float[] prevX;
    float[] prevY;    

    public MultiTouchEventHandler(BlueGinInput input)
    {
        mInput = input;
        prevX = new float[MAX_POINTS];
        prevY = new float[MAX_POINTS];
    }

    public boolean onTouchEvent(MotionEvent ev)
    {
        final int action = ev.getActionMasked();

        switch (action) {
            //  Touches began
            case MotionEvent.ACTION_DOWN: {
                final float x = ev.getX();
                final float y = ev.getY();
                final int id = ev.getPointerId(0);
                mInput.addTouchEvent(0, x, y, x, y, id);
                prevX[id] = x;
                prevY[id] = y;
                break;
            }
            case MotionEvent.ACTION_POINTER_DOWN: {
                final int pointerIndex = ev.getActionIndex();
                final float x = ev.getX(pointerIndex);
                final float y = ev.getY(pointerIndex);
                final int id = ev.getPointerId(pointerIndex);
                mInput.addTouchEvent(0, x, y, x, y, id);
                prevX[id] = x;
                prevY[id] = y;
                break;
            }

            //  Touches moved
            case MotionEvent.ACTION_MOVE: {
                //  XXX getActionIndex() does not indicate when a secondary pointer moves
                for (int index=0; index < ev.getPointerCount(); ++index) {
                    float x = ev.getX(index);
                    float y = ev.getY(index);
                    int id = ev.getPointerId(index);
                    mInput.addTouchEvent(1, x, y, prevX[id], prevY[id], id);
                    prevX[id] = x;
                    prevY[id] = y;
                }
                break;
            }

            //  Touches ended
            case MotionEvent.ACTION_UP: {
                final float x = ev.getX();
                final float y = ev.getY();
                final int id = ev.getPointerId(0);
                mInput.addTouchEvent(2, x, y, prevX[id], prevY[id], id);
                break;
            }
            case MotionEvent.ACTION_POINTER_UP: {
                // Extract the index of the pointer that left the touch sensor
                final int pointerIndex = ev.getActionIndex();
                final float x = ev.getX(pointerIndex);
                final float y = ev.getY(pointerIndex);
                final int id = ev.getPointerId(pointerIndex);
                mInput.addTouchEvent(2, x, y, prevX[id], prevY[id], id);
                break;
            }
        }

        return true;
    }
}

