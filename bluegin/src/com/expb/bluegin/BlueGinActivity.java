package com.expb.bluegin;

import android.os.Bundle;
import android.os.PowerManager.WakeLock;

import android.app.Activity;
import android.util.Log;

import android.view.KeyEvent;
import android.view.MotionEvent;

import android.content.res.AssetManager;
import android.content.res.AssetFileDescriptor;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.graphics.PixelFormat;

import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;

import com.expb.bluegin.BlueGinView;
import com.expb.bluegin.TouchEventHandler;

import java.util.ArrayList;

public class BlueGinActivity extends Activity
{
    private static final int MAX_STREAMS = 16;

    private static final String TAG = "BlueGin";

    //  globals for use in static methods
    public static BlueGinActivity app = null;

    public  ArrayList<MediaPlayer> mediaPlayers = null;
    public  MediaPlayer mediaPlayer = null;
    public  SoundPool   soundPool   = null;

    private BlueGinView       mView;
    private BlueGinInput      mInput;
    private TouchEventHandler mTouchHandler;

    private boolean mKeyboardVisible = false;

    /** Called when the activity is (re)started. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        app = this;
        super.onCreate(savedInstanceState);

        setVolumeControlStream(AudioManager.STREAM_MUSIC);
    }

    @Override
    public void onStart()
    {
        super.onStart();
        initSoundPool();
        mInput        = new BlueGinInput(getApplication());
        mTouchHandler = TouchEventHandler.create(mInput);
        mView         = new BlueGinView(getApplication(), mInput);

        mediaPlayers  = new ArrayList<MediaPlayer>();

        setContentView(mView);

        Native.create();
    }

    public BlueGinView getView()
    {
        return mView;
    }

    public boolean getKeyboardVisible()
    {
        return mKeyboardVisible;
    }

    public void setKeyboardVisible(boolean on)
    {
        mKeyboardVisible = on;
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

        Log.v(TAG, "onStop() called");
        //  Force hide the keyboard if visible
        BlueGinAndroid.keyboard_toggle(false);

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

    public synchronized boolean onKeyDown(int keycode, KeyEvent e) 
    {

        if (keycode == KeyEvent.KEYCODE_VOLUME_UP || 
            keycode == KeyEvent.KEYCODE_VOLUME_DOWN ||
            keycode == KeyEvent.KEYCODE_VOLUME_MUTE) {
            return super.onKeyDown(keycode, e);
        }

        Log.v("keydown", Character.toString(Character.toChars(e.getUnicodeChar())[0]));
        int unicodeChar = e.getUnicodeChar();
        // XXX translate and send modifiers
        mInput.addKeyEvent(true, unicodeChar, 0, keycode);

        return true;
    }

    // XXX add implementation of onKeyUp for completeness

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


