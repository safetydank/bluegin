#include "flx/flxG.h"
#include "flx/sound.h"

using namespace flx;

extern FlxGlobal FlxG;

Sound::Sound() : Object(0,0,0,0)
{
    _point2.set(0,0);
    // _transform = new SoundTransform();
    init();
    fixed = true; //no movement usually
}

Sound::Sound(bluegin::AudioPtr source) : Object(0,0,0,0)
{
    _point2.set(0,0);
    _sound = source;
    init();
    fixed = true;
}

void Sound::init()
{
    // _transform.pan = 0;
    // _sound = null;
    _position = 0;
    _volume = 1.0;
    _volumeAdjust = 1.0;
    _looped = false;
    _core = NULL;
    _radius = 0;
    _pan = false;
    _fadeOutTimer = 0;
    _fadeOutTotal = 0;
    _pauseOnFadeOut = false;
    _fadeInTimer = 0;
    _fadeInTotal = 0;
    active = false;
    visible = false;
    solid = false;
    // playing = false;
    name = string("");
    artist = string("");
}

bool Sound::getPlaying()
{
    return _sound->isPlaying();
}

void Sound::proximity(float X,float Y,Object* Core,float Radius,bool Pan)
{
    x = X;
    y = Y;
    _core = Core;
    _radius = Radius;
    _pan = Pan;
}

void Sound::play()
{
    if(_position < 0)
        return;
    _sound->setLooping(_looped);
    _sound->play();
    _position = 0;
}

void Sound::pause()
{
    // if(_channel == null)
    // {
    //     _position = -1;
    //     return;
    // }
    // _position = _channel.position;
    // _channel.stop();
    // if(_looped)
    // {
    //     while(_position >= _sound.length)
    //         _position -= _sound.length;
    // }
    // _channel = null;
    if (_sound)
        _sound->pause();
    // playing = false;
}

void Sound::stop()
{
    _position = 0;
    if (_sound) {
        _sound->stop();
        active = false;
        // playing = false;
    }
    // if(_channel != null)
    // {
    //     _channel.stop();
    //     stopped();
    // }
}

void Sound::fadeOut(float Seconds,bool PauseInstead)
{
    _pauseOnFadeOut = PauseInstead;
    _fadeInTimer = 0;
    _fadeOutTimer = Seconds;
    _fadeOutTotal = _fadeOutTimer;
}

void Sound::fadeIn(float Seconds)
{
    _fadeOutTimer = 0;
    _fadeInTimer = Seconds;
    _fadeInTotal = _fadeInTimer;
    play();
}

float Sound::getVolume()
{
    return _volume;
}

void Sound::setVolume(float Volume)
{
    _volume = Volume;
    if(_volume < 0)
        _volume = 0;
    else if(_volume > 1)
        _volume = 1;
    updateTransform();
}

void Sound::updateSound()
{
    if(_position != 0)
        return;

    float radial = 1.0f;
    float fade = 1.0f;

    // //Distance-based volume control
    // if(_core != null)
    // {
    //     var _point:FlxPoint = new FlxPoint();
    //     var _point2:FlxPoint = new FlxPoint();
    //     _core.getScreenXY(_point);
    //     getScreenXY(_point2);
    //     var dx:Number = _point.x - _point2.x;
    //     var dy:Number = _point.y - _point2.y;
    //     radial = (_radius - Math.sqrt(dx*dx + dy*dy))/_radius;
    //     if(radial < 0) radial = 0;
    //     if(radial > 1) radial = 1;

    //     if(_pan)
    //     {
    //         var d:Number = -dx/_radius;
    //         if(d < -1) d = -1;
    //         else if(d > 1) d = 1;
    //         _transform.pan = d;
    //     }
    // }

    //Cross-fading volume control
    if(_fadeOutTimer > 0)
    {
        _fadeOutTimer -= FlxG.elapsed;
        if(_fadeOutTimer <= 0)
        {
            if(_pauseOnFadeOut)
                pause();
            else
                stop();
        }
        fade = _fadeOutTimer/_fadeOutTotal;
        if(fade < 0) fade = 0;
    }
    else if(_fadeInTimer > 0)
    {
        _fadeInTimer -= FlxG.elapsed;
        fade = _fadeInTimer/_fadeInTotal;
        if(fade < 0) fade = 0;
        fade = 1 - fade;
    }

    _volumeAdjust = radial*fade;
    updateTransform();
}

void Sound::update()
{
    Object::update();
    updateSound();			
}

void Sound::destroy()
{
    if(active)
        stop();
}

void Sound::updateTransform()
{
    float volume = FlxG.getMuteValue() * FlxG.getVolume() * _volume * _volumeAdjust;
    // _transform.volume = FlxG.getMuteValue()*FlxG.volume*_volume*_volumeAdjust;
    // if(_channel != null)
    //     _channel.soundTransform = _transform;
}

