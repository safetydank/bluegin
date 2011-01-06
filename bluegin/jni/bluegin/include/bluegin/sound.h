#pragma once

namespace bluegin {

enum SoundType {
    SOUND_TYPE,
    MUSIC_TYPE
};

struct AudioSource
{
public:
    AudioSource(SoundType type, string path);

    SoundType type;
    string path;

    int soundID;
};

typedef shared_ptr<AudioSource> AudioSourcePtr;

//  Sound handles

class Audio
{
public:
    Audio(AudioSourcePtr source);

    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual bool isPlaying() { return false; }
    void setVolume(float left, float right);
    void setLooping(bool loop);

protected:
    virtual void updateSound() { }

    float mVolumeL, mVolumeR;
    bool mLoop;
    AudioSourcePtr mSource;
};

class Sound : public Audio
{
public:
    Sound(AudioSourcePtr source, int streamID);
    //  implementations
    virtual void play();
    virtual void stop();
    virtual void pause();
    virtual void resume();

protected:
    int mStreamID;
    bool active;
    float lastPlayed;

    virtual void updateSound();
};

class Music : public Audio
{
public:
    Music(AudioSourcePtr source);
    virtual void play();
    virtual void stop();
    virtual void pause();
    virtual void resume();
    virtual bool isPlaying();

protected:
};

typedef shared_ptr<Audio> AudioPtr;

}
