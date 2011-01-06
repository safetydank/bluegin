#pragma once

#include "flx/object.h"
#include "bluegin/sound.h"

namespace flx {


/**
 * XXX currently unused, FlxG sound methods just wrap the Bluegin sound classes
 * This is the universal flixel sound object, used for streaming, music, and sound effects.
 */
class Sound : public Object
{
public:
    /**
     * The ID3 song name.  Defaults to null.  Currently only works for streamed sounds.
     */
    string name;
    /**
     * The ID3 artist name.  Defaults to null.  Currently only works for streamed sounds.
     */
    string artist;
    
protected:
    bool _init;
    bluegin::AudioPtr _sound;
    int _channel;
    // protected var _transform:SoundTransform;
    float _position;
    float _volume;
    float _volumeAdjust;
    bool _looped;
    Object* _core;
    float _radius;
    bool _pan;
    float _fadeOutTimer;
    float _fadeOutTotal;
    bool _pauseOnFadeOut;
    float _fadeInTimer;
    float _fadeInTotal;
    ci::Vec2f _point2;
    
public:
    /**
     * The Sound constructor gets all the variables initialized, but NOT ready to play a sound yet.
     */
    Sound();
    
    Sound(bluegin::AudioPtr source);
    /**
     * An internal function for clearing all the variables used by sounds.
     */
    void init();

    /**
     * Whether the sound is currently playing or not.
     */
    bool getPlaying();

    /**
     * Not implemented
     *
     * Call this function if you want this sound's volume to change
     * based on distance from a particular Object.
     * 
     * @param	X		The X position of the sound.
     * @param	Y		The Y position of the sound.
     * @param	Core	The object you want to track.
     * @param	Radius	The maximum distance this sound can travel.
     */
    void proximity(float X,float Y,Object* Core,float Radius,bool Pan=true);
    
    /**
     * Call this function to play the sound.
     */
    void play();
    
    /**
     * Call this function to pause this sound.
     */
    void pause();
    
    /**
     * Call this function to stop this sound.
     */
    void stop();
    
    /**
     * Call this function to make this sound fade out over a certain time interval.
     * 
     * @param	Seconds			The amount of time the fade out operation should take.
     * @param	PauseInstead	Tells the sound to pause on fadeout, instead of stopping.
     */
    void fadeOut(float Seconds,bool PauseInstead=false);
    
    /**
     * Call this function to make a sound fade in over a certain
     * time interval (calls <code>play()</code> automatically).
     * 
     * @param	Seconds		The amount of time the fade-in operation should take.
     */
    void fadeIn(float Seconds);
    
    /**
     * Set <code>volume</code> to a value between 0 and 1 to change how this sound is.
     */
    float getVolume();
    
    /**
     * @private
     */
    void setVolume(float Volume);
    
    /**
     * Internal function that performs the actual logical updates to the sound object.
     * Doesn't do much except optional proximity and fade calculations.
     */
    void updateSound();

    /**
     * The basic game loop update function.  Just calls <code>updateSound()</code>.
     */
    virtual void update();
    
    /**
     * The basic class destructor, stops the music and removes any leftover events.
     */
    virtual void destroy();
    
    /**
     * An internal function used to help organize and change the volume of the sound.
     */
    void updateTransform();
};

}
