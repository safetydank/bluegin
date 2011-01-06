#pragma once

#include "cinder/gl/Texture.h"
#include "bluegin/bluegin.h"
#include "bluegin/Graphic.h"
#include "bluegin/hgeFont.h"
#include "bluegin/Sound.h"

namespace bluegin {

struct ResourceConfig;

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    void loadResourceConfig(const char* configPath);
    bool loadTexture(ResourceConfig& rc);
    bool loadGraphic(ResourceConfig& rc);
    bool loadFont(ResourceConfig& rc);
    bool loadSound(SoundType type, ResourceConfig& rc);
    bool loadMusic(ResourceConfig& rc);

    ci::gl::Texture texture(string texName);
    Graphic         graphic(string graphicName);
    FontPtr         font(string fontName);
    AudioSourcePtr  sound(string soundName);

    //  Sound API designed for two scenarios:
    //  
    //  1.  One set of samples used throughout the app.  In this case call
    //      primeAllSounds() once at the start of the App - all sounds are loaded
    //      and added to the SoundPool.
    //
    //  2.  Different sound sets used in different states (e.g. levels/worlds).
    //      Call resetSounds() to clear the soundpool at the beginning of state
    //      create() and then call primeSound to add each sound source for playback.
    //
    //  These calls have no effect on MUSIC_TYPE sound sources.

    //  Prime a sound for playback
    void primeSound(AudioSourcePtr sound);
    //  Prime all the loaded sounds for playback
    void primeAllSounds();
    //  Unprime all the sounds and initializes a new SoundPool
    void resetSounds();

protected:
    map<string, ci::gl::Texture> mTextures;
    map<string, Graphic>         mGraphics;
    map<string, FontPtr>         mFonts;
    map<string, AudioSourcePtr>  mSounds;
};

typedef shared_ptr<ResourceManager> ResourceManagerPtr;

}
