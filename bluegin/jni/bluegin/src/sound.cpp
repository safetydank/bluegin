#include "bluegin/bluegin.h"
#include "bluegin/sound.h"
#include "cinder/CinderMath.h"

using namespace bluegin;
using namespace ci;

AudioSource::AudioSource(SoundType soundType, string soundPath)
    : type(soundType), path(soundPath), soundID(-1)
{
}

Audio::Audio(AudioSourcePtr source) 
    : mVolumeL(1.0f), mVolumeR(1.0f), mLoop(false), mSource(source)
{
}

void Audio::setVolume(float left, float right)
{
    // XXX clamp
    mVolumeL = math<float>::clamp(left, 0, 1.0f);
    mVolumeR = math<float>::clamp(right, 0, 1.0f);
    updateSound();
}

void Audio::setLooping(bool loop)
{
    mLoop = loop;
}

Sound::Sound(AudioSourcePtr source, int streamID) 
    : Audio(source), mStreamID(streamID)
{
    if (!bool(mSource) || mSource->soundID == -1) {
        Log("Error initializing sound from audio source");
        mSource = AudioSourcePtr();
    }
}

//  implementations
void Sound::play()
{
#ifdef ANDROID
    // XXX implement!
    // if (mSource)
    // bluegin_sound_start(mStreamID);
#endif
}

void Sound::stop()
{
#ifdef ANDROID
    if (mSource)
        bluegin_sound_stop(mStreamID);
#endif
}

void Sound::pause()
{
#ifdef ANDROID
    if (mSource)
        bluegin_sound_pause(mStreamID);
#endif
}

void Sound::resume()
{
#ifdef ANDROID
    if (mSource)
        bluegin_sound_resume(mStreamID);
#endif
}

void Sound::updateSound()
{
    if (mSource)
        bluegin_sound_set_volume(mStreamID, mVolumeL, mVolumeR);
}

Music::Music(AudioSourcePtr source) : Audio(source)
{
    if (source->type != MUSIC_TYPE) {
        Log("Error - initialized Music() from a non-music resource");
        mSource = AudioSourcePtr();
    }
}

void Music::play()
{
#ifdef ANDROID
    if (mSource) 
        bluegin_music_play(mSource->path.c_str());
#endif
}

void Music::stop()
{
#ifdef ANDROID
    if (mSource) 
        bluegin_music_stop();
#endif
}

bool Music::isPlaying()
{
#ifdef ANDROID
    if (mSource)
        return bluegin_music_is_playing();
#endif
    return false;
}

void Music::pause()
{
}

void Music::resume()
{
}

