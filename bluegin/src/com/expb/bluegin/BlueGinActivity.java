package com.expb.bluegin;

import android.os.Bundle;
import android.app.Activity;
import android.view.MotionEvent;
import android.util.Log;

import android.content.res.AssetManager;
import android.content.res.AssetFileDescriptor;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.graphics.PixelFormat;

import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;

import android.os.SystemClock;

import com.expb.bluegin.BlueGinView;
import com.expb.bluegin.TouchEventHandler;

public class BlueGinActivity extends Activity
{
    private static final int MAX_STREAMS = 16;

    private static final String TAG = "BlueGin";

    //  globals for use in static methods
    public static BlueGinActivity app = null;

    public  MediaPlayer mediaPlayer = null;
    public  SoundPool   soundPool   = null;

    private BlueGinView       mView;
    private BlueGinInput      mInput;
    private TouchEventHandler mTouchHandler;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        app = this;
        super.onCreate(savedInstanceState);

        initSoundPool();
        mInput = new BlueGinInput(getApplication());
        mTouchHandler = TouchEventHandler.create(mInput);
        mView  = new BlueGinView(getApplication(), mInput);
        setContentView(mView);

        Native.create();
    }

    @Override
    public void onStart()
    {
        super.onStart();
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        mView.onResume();
    }

    @Override
    protected void onPause()
    {
        super.onPause();
        mView.onPause();

        //  Save state
        Native.pause();
    }

    protected void onStop()
    {
        super.onStop();

        //  Stop and free any playing sounds
        BlueGinAndroid.music_stop();
        resetSoundPool();

        Native.cleanup();

        mInput.stop();
    }

    public synchronized boolean onTouchEvent(final MotionEvent ev) 
    {
        boolean ret = mTouchHandler.onTouchEvent(ev);

        //  synchronize input with update/render thread
        //  this limits the rate of input events to display rate
        synchronized (mView) {
            try {
                mView.wait(1000L);
            } 
            catch (InterruptedException ex) {
            }
        }
        return true;
    }

    public void resetSoundPool()
    {
        if (soundPool != null) {
            soundPool.release();
            soundPool = null;
        }
    }

    public void initSoundPool()
    {
        soundPool = new SoundPool(MAX_STREAMS, AudioManager.STREAM_MUSIC, 0);
    }
}


