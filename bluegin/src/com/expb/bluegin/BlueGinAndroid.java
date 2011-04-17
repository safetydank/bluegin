package com.expb.bluegin;

import android.util.Log;

import android.content.res.AssetManager;
import android.content.res.AssetFileDescriptor;
import android.content.Context;

import android.view.inputmethod.InputMethodManager;

import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.graphics.Canvas;

import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.SoundPool;

import android.opengl.GLUtils;

import java.io.File;
import java.io.InputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import javax.microedition.khronos.opengles.GL10;

import java.util.Iterator;

import com.expb.bluegin.BlueGinActivity;

public class BlueGinAndroid
{
    private static final String TAG = "BlueGin";

    private static GL10 mGL = null;
    public static void setGL(GL10 gl) { mGL = gl; }

    private static float musicVolumeL;
    private static float musicVolumeR;

    // Java callbacks, called from native code
    public static int load_texture(String fname, int[] dim) 
    {
        AssetManager am = BlueGinActivity.app.getAssets(); 

        try {
            InputStream stream = am.open(fname);
            Bitmap bitmap = BitmapFactory.decodeStream(stream);
            dim[0] = bitmap.getWidth();
            dim[1] = bitmap.getHeight();

            int[] textures = new int[1];
            mGL.glGenTextures(1, textures, 0);
            int textureID = textures[0];
            mGL.glBindTexture(GL10.GL_TEXTURE_2D, textureID);
            GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, bitmap, 0);
            Log.v(TAG, "load_texture: "+fname+" texid: "+textureID);
            return textureID;
        } catch(IOException e) {
            return 0;
        }
    }

    private static int nearest_power_of_2(int n) 
    {
        if(n > 1024) return 2048;
        if(n > 512) return 1024;
        if(n > 256) return 512;
        if(n > 128) return 256;
        if(n > 64) return 128;
        if(n > 32) return 64;
        if(n > 16) return 32;
        if(n > 8) return 16;
        if(n > 4) return 8;
        if(n > 2) return 4;
        if(n > 1) return 2;
        return 1;
    }

    // Load a non-power-of-two texture and create a POT texture
    // XXX combine with load_texture?
    public static int load_texture_npot(String fname, int[] dim)
    {
        AssetManager am = BlueGinActivity.app.getAssets(); 

        try {
            InputStream stream = am.open(fname);
            Bitmap original = BitmapFactory.decodeStream(stream);
            int origWidth  = dim[0] = original.getWidth();
            int origHeight = dim[1] = original.getHeight();

            if (origWidth > 2048 || origHeight > 2048) {
                Log.v(TAG, "Error: texture too large for npot (>2048 pixels long/high)");
                return 0;
            }

            int width  = dim[2] = nearest_power_of_2(origWidth);
            int height = dim[3] = nearest_power_of_2(origHeight);

            Bitmap bitmap;

            if (width == origWidth && height == origHeight) {
                bitmap = original;
            }
            else {
                bitmap = Bitmap.createBitmap(width, height, original.getConfig());
                Canvas canvas = new Canvas(bitmap);
                canvas.drawBitmap(original, 0, 0, null);
            }

            int[] textures = new int[1];
            mGL.glGenTextures(1, textures, 0);
            int textureID = textures[0];
            mGL.glBindTexture(GL10.GL_TEXTURE_2D, textureID);
            GLUtils.texImage2D(GL10.GL_TEXTURE_2D, 0, bitmap, 0);
            Log.v(TAG, "load_texture: "+fname+" texid: "+textureID);
            return textureID;
        } catch(IOException e) {
            Log.v(TAG, "Error loading texture "+fname+", null returned");
            return 0;
        }
    }

    public static byte[] load_resource(String fname)
    {
        Log.v(TAG, "load_resource: "+fname);
        AssetManager am = BlueGinActivity.app.getAssets();
        try {
            InputStream stream = am.open(fname);

            ByteArrayOutputStream bout = new ByteArrayOutputStream();
            byte[] buffer = new byte[1024];
            int len;
            while ((len = stream.read(buffer)) > 0) {
                bout.write(buffer, 0, len);
            }
            // Log.v(TAG, "resource load length "+bout.toByteArray().length);
            return bout.toByteArray();
        } catch(IOException e) {
            Log.v(TAG, "load_resource failed");
            return new byte[0];
        }
    }

    public static void keyboard_toggle(boolean show) 
    {
        BlueGinActivity app = BlueGinActivity.app;
        InputMethodManager mgr = (InputMethodManager) app.getSystemService(Context.INPUT_METHOD_SERVICE);
        mgr.hideSoftInputFromWindow(app.getView().getWindowToken(), 0);
        if (show) {
            mgr.toggleSoftInput(InputMethodManager.SHOW_FORCED,0);
            // On the Nexus One, SHOW_FORCED makes it impossible
            // to manually dismiss the keyboard.
            // On the Droid SHOW_IMPLICIT doesn't bring up the keyboard.
        }
    }

    // JNI to play music, etc
    public static void music_play(String fname, boolean looping) 
    {
        BlueGinActivity app = BlueGinActivity.app;

        Log.v("music_play",fname);
        AssetManager am = app.getAssets();
        try {
            Log.v(TAG, "Playing music file: "+fname);
            AssetFileDescriptor fd = am.openFd(fname);
            MediaPlayer mp = new MediaPlayer();
            mp.setDataSource(fd.getFileDescriptor(), fd.getStartOffset(), fd.getLength());
            mp.setVolume(musicVolumeL, musicVolumeR);
            fd.close();
            mp.setLooping(looping);
            mp.prepare();
            mp.start();
            app.mediaPlayers.add(mp);

            //  clear any stopped music streams
            Iterator itr = BlueGinActivity.app.mediaPlayers.iterator();
            while (itr.hasNext()) {
                MediaPlayer player = (MediaPlayer) itr.next();
                if (player != null && !player.isPlaying()) {
                    itr.remove();
                }
            }
        } catch(IOException e) { 
            Log.v(TAG, "Error playing music file: "+fname);
        }
    }

    public static void music_stop() 
    {
        //  if (BlueGinActivity.app.mediaPlayer == null) { return; }
        // BlueGinActivity.app.mediaPlayer.stop();

        Iterator itr = BlueGinActivity.app.mediaPlayers.iterator();
        while (itr.hasNext()) {
            MediaPlayer player = (MediaPlayer) itr.next();
            if (player != null && player.isPlaying()) {
                player.stop();
                itr.remove();
            }
        }
    }

    public static boolean music_is_playing()
    {
        // if (BlueGinActivity.app.mediaPlayer == null) { return false; }
        // return BlueGinActivity.app.mediaPlayer.isPlaying();

        Iterator itr = BlueGinActivity.app.mediaPlayers.iterator();
        while (itr.hasNext()) {
            MediaPlayer player = (MediaPlayer) itr.next();
            if (player != null && player.isPlaying()) {
                return true;
            }
            else {
                itr.remove();
            }
        }
        return false;
    }

    public static void music_volume(float volumeL, float volumeR) 
    {
        // if (BlueGinActivity.app.mediaPlayer == null) { return; }
        // BlueGinActivity.app.mediaPlayer.setVolume(volumeL,volumeR);
        musicVolumeL = volumeL;
        musicVolumeR = volumeR;

        Iterator itr = BlueGinActivity.app.mediaPlayers.iterator();
        while (itr.hasNext()) {
            MediaPlayer player = (MediaPlayer) itr.next();
            if (player != null && player.isPlaying()) {
                player.setVolume(volumeL, volumeR);
            }
            else {
                itr.remove();
            }
        }
    }

    public static void music_pause(boolean resume)
    {
        Iterator itr = BlueGinActivity.app.mediaPlayers.iterator();
        while (itr.hasNext()) {
            MediaPlayer player = (MediaPlayer) itr.next();
            if (player != null && !resume) {
                player.pause();
            }
            else if (player != null && resume) {
                player.start();
            }
        }
    }

    public static void sound_init()
    {
        //  initialize volume settings
        musicVolumeL = 1.0f;
        musicVolumeR = 1.0f;

        BlueGinActivity.app.resetSoundPool();
        BlueGinActivity.app.initSoundPool();
    }

    public static int sound_load(String fname) 
    {
        Log.v(TAG, "sound_load: "+fname);
        AssetManager am = BlueGinActivity.app.getAssets();
        try {
            AssetFileDescriptor fd = am.openFd(fname);
            int sid = BlueGinActivity.app.soundPool.load(fd.getFileDescriptor(),fd.getStartOffset(),fd.getLength(),1);
            return sid;
        } catch(IOException e) { }
        return 0;
    }

    public static int sound_play(int sid, float leftVolume, float rightVolume, int priority, int loop, float rate) 
    {
        return BlueGinActivity.app.soundPool.play(sid, leftVolume, rightVolume, priority, loop, rate);
    }

    public static void sound_stop(int streamID) 
    {
        BlueGinActivity.app.soundPool.stop(streamID);
    }

    public static void sound_set_volume(int streamID, float leftVolume, float rightVolume) 
    {
        BlueGinActivity.app.soundPool.setVolume(streamID, leftVolume, rightVolume);
    }

    public static void sound_set_pitch(int streamID, float rate) 
    {
        BlueGinActivity.app.soundPool.setRate(streamID, rate);
    }

    public static void sound_pause(int streamID) 
    {
        BlueGinActivity.app.soundPool.pause(streamID);
    }

    public static void sound_resume(int streamID) 
    {
        BlueGinActivity.app.soundPool.resume(streamID);
    }

    public static String get_docdir()
    {
        File fdir = BlueGinActivity.app.getFilesDir();
        return fdir.getAbsolutePath();
    }
}

