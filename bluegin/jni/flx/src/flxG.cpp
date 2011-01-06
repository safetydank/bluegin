#include "flx/flx.h"
#include "flx/flxG.h"
#include "flx/sprite.h"

#include <cmath>

using namespace ci;
using namespace flx;
using namespace bluegin;

extern FlxGlobal FlxG;

const int MAX_STREAMS = 16;

State* FlxGlobal::getState()
{
    return FlxG.game->_state.get();
}

void FlxGlobal::setState(StatePtr state)
{
    FlxG.game->switchState(state);
}

void FlxGlobal::setGameData(IGame* game, float gameSizeX, float gameSizeY, Vec2f zoom)
{
    FlxG.game = game;
    FlxG.width = gameSizeX;
    FlxG.height = gameSizeY;
    FlxG.zoom = zoom;
    FlxG.timeScale = 1.0f;

    // XXX without this reset the android version renders invisible.  probably a retained
    // VBO object, had a similar problem with VBO assignment to a static variable earlier.
    FlxG.resources.reset();
    FlxG.resources = ResourceManagerPtr(new ResourceManager());
    FlxG.renderer.reset();
    FlxG.renderer = RendererPtr(new Renderer());

    FlxG.defaultFont = NULL;

    FlxU::setWorldBounds();

    //  Create a block allocator
    FlxG.blockAllocator = new b2BlockAllocator();

    //  Device identifiers
    //  XXX populate these correctly
    iPad = false;
    iPhone3G = false;
    iPhone1G = false;
    iPodTouch1G = false;
}

void FlxGlobal::follow(ObjectPtr Target, double Lerp)
{
    if (!Target)
        return;

    followTarget = Target;
    Sprite& followSprite = static_cast<Sprite&>(*Target);
    followLerp = Lerp;
    _scrollTarget.x = (width * 0.5f)-followSprite.x-(followSprite.width * 0.5f)+followSprite.offset.x;
    _scrollTarget.y = (height * 0.5f)-followSprite.y-(followSprite.height * 0.5f)+followSprite.offset.y;
    scroll.x = _scrollTarget.x;
    scroll.y = _scrollTarget.y;
    doFollow();
}

void FlxGlobal::followAdjust(float LeadX, float LeadY)
{
    _followLead.set(LeadX,LeadY);
    followLead = &_followLead;
}

void FlxGlobal::followBounds(int MinX, int MinY, int MaxX, int MaxY, 
                             bool UpdateWorldBounds)
{
    _followMin.set(-MinX,-MinY);
    _followMax.set(-MaxX+width,-MaxY+height);

    followMin = &_followMin;
    followMax = &_followMax;

    if(_followMax.x > _followMin.x)
        _followMax.x = _followMin.x;
    if(_followMax.y > _followMin.y)
        _followMax.y = _followMin.y;
    if(UpdateWorldBounds)
        FlxU::setWorldBounds(-MinX,-MinY,-MinX+MaxX,-MinY+MaxY);
    doFollow();
}

void FlxGlobal::doFollow()
{
    if(followTarget)
    {
        Sprite& followSprite = static_cast<Sprite&>(*followTarget);

        _scrollTarget.x = (width*0.5f)-followSprite.x-(followSprite.width*0.5f)+followSprite.offset.x;
        _scrollTarget.y = (height*0.5f)-followSprite.y-(followSprite.height*0.5f)+followSprite.offset.y;
        if((followLead != NULL) /*&& (followSprite is FlxSprite)*/)
        {
            _scrollTarget.x -= followSprite.velocity.x*_followLead.x;
            _scrollTarget.y -= followSprite.velocity.y*_followLead.y;
        }
        scroll += followLerp * elapsed * (_scrollTarget - scroll);

#ifndef ANDROID
        //  XXX snap to 1/10th of a pixel on Windows to avoid texture edge fringing
        scroll = Vec2d( ((int)(scroll.x * 10)) / 10.0f, ((int)(scroll.y * 10)) / 10.0f );
#endif

        if(followMin != NULL)
        {
            if(scroll.x > _followMin.x)
                scroll.x = _followMin.x;
            if(scroll.y > _followMin.y)
                scroll.y = _followMin.y;
        }

        if(followMax != NULL)
        {
            if(scroll.x < _followMax.x)
                scroll.x = _followMax.x;
            if(scroll.y < _followMax.y)
                scroll.y = _followMax.y;
        }
    }
}

void FlxGlobal::unfollow()
{
    followTarget = ObjectPtr();
    followLead = NULL;
    followLerp = 1.0;
    followMin = NULL;
    followMax = NULL;
    scroll.x = scroll.y = 0;
    _scrollTarget.x = _scrollTarget.y = 0;
}

AudioPtr FlxGlobal::play(AudioSourcePtr source, float volume, bool looped)
{
    Log("FlxG::play soundId %d", source->soundID);
    int soundID = source->soundID;

    if (soundID != -1) {
        Log("Attempting to play sound %d", soundID);
        int streamID = bluegin_sound_play(soundID, volume, volume, 0, 
                looped ? -1 : 0, 1.0f);
        Log("Received stream ID %d", streamID);
        return AudioPtr(new Sound(source, streamID));
    }
    else {
        Log("Skipping unloaded sound %s - try loading it first!", 
                source->path.c_str());
    }

    return AudioPtr();
}

AudioPtr FlxGlobal::playMusic(AudioSourcePtr source, float volume)
{
    if (FlxG.music) {
        FlxG.music->stop();
    }
    FlxG.music = AudioPtr(new Music(source));
    music->play();
    return music;
}

bool FlxGlobal::getMute()
{
    return _mute;
}

void FlxGlobal::setMute(bool Mute)
{
    _mute = Mute;
    changeSounds();
}

float FlxGlobal::getVolume()
{
    return _volume;
}

void  FlxGlobal::setVolume(float Volume)
{
    _volume = math<float>::clamp(Volume, 0, 1.0f);
    changeSounds();
}

float FlxGlobal::getMuteValue()
{
    if(_mute)
        return 0;
    else
        return 1.0f;
}

void FlxGlobal::changeSounds()
{
    //  Set AudioManager media stream volume
}

void FlxGlobal::updateSounds()
{
    //  Set audio transform (volume/panning) of every sound in a sound pool
}

