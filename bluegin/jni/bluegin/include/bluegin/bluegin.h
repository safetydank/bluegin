#pragma once

/**
\mainpage BlueGin Documentation

\section overview_sec Overview

The BlueGin game engine is built on a loosely coupled stack of components, the
major parts being Cinder, BlueGin and Flx.

Cinder handles lifecycle, math, input and rendering duties.  BlueGin provides
platform specific access to resources and loading facilities.  Flx is the game
engine, based on a native port of Flixel.

For game developers most of the relevant classes can be found in the flx
namespace.

Main repository - http://github.com/safetydank/bluegin

Contact - dan@expandingbrain.com

*/

#ifdef ANDROID
#include "bluegin/android.h"
#include "yasper.h"
#include <stdlib.h>
namespace std {
    //  XXX HACK to enable std::string to compile, will exit immediately on length error
    //  (exceptions not supported with NDK)
    inline void __stl_throw_length_error(const char*) { exit(1); };
}
#else
#include "boost/smart_ptr.hpp"
#define Log(...) ::printf(__VA_ARGS__)
#include <boost/cstdint.hpp>
//  VBO index type is uint32_t on Windows and unsigned short on Android
typedef boost::uint32_t index_t;
#endif

int   bluegin_load_texture(const char *s, int* width, int* height);
void* bluegin_load_resource(const char *s, int* size);

void  bluegin_music_play(const char* fname);
void  bluegin_music_stop();
bool  bluegin_music_is_playing();
void  bluegin_sound_init();
int   bluegin_sound_load(const char* fname);
int   bluegin_sound_play(int sid, float leftVolume, float rightVolume, int priority, int loop, float rate);
void  bluegin_sound_stop(int streamID);
void  bluegin_sound_set_pitch(int streamID, float rate);
void  bluegin_sound_set_volume(int streamID, float leftVolume, float rightVolume);
void  bluegin_sound_pause(int streamID);
void  bluegin_sound_resume(int streamID);

using boost::shared_ptr;

#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;

