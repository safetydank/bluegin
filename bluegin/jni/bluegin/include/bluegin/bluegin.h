#pragma once

/**
\mainpage BlueGin Documentation

\section overview_sec Overview

BlueGin is a native C++ game engine for mobile devices.  It is built on ports
of Cinder and Flixel, two popular libraries for creative and game applications.

BlueGin games are compatible with Android 1.6 and above.  Graphics are
rendered with OpenGL ES 1.5.

For game developers most of the relevant classes can be found in the \a flx
namespace.

\subsection portable_ss Portable and self contained

BlueGin has minimal external dependencies - a working copy of the NDK (R5) and
Boost is all that's needed to build an Android app.

The distribution includes a subset of the Cinder library and full versions of
Box2D and RapidXML.

\subsection resources_ss Resource management and texture packing

A unified resource manager provides access to image, audio and font assets.
The asset pipeline includes a Python texture packer for creating texture
atlases that use GPU resources efficiently and render fast.

\subsection gamestate_ss Game state management

Games are written as a set of (FSM) state classes with hooks for
initialization, transitioning and cleanup.

\subsection entities_ss Entities (objects and sprites)

Game entities derive from a unified hierarchy.  Custom game entities can derive
from a minimal flx::Object class or a flx::Sprite class that manages rendering
and animation.

\subsection groups_ss Group management

Game entities can be added to a Group class for organizational purposes.
Groups are used throughout to manage rendering lists, collision detection
groups and bulk entity operations.

\subsection collision_ss Collision detection

BlueGin uses a quadtree to perform efficient collision detection and resolution
in 2D.  Collisions may be checked by a custom callback or by overriding
callbacks on the entities themselves.

\subsection tilemaps_ss Tilemaps

Support for general tilemaps, including specialized collision detection and
autogeneration of tilemap areas.

BlueGin includes an importer for Ogmo Editor tilemaps.

\subsection particles_ss Particles

A simple emitter class is included, with hooks for creating variations of
particle effects.

\subsection audio_ss Audio

A native interface wrapping Android's MediaPlayer and SoundPool libraries
provides low latency playback, resampling and volume control of sound and music
files.

\subsection fonts_ss Fonts

Fast bitmap font rendering, with support for importing HGE (Haaf's Game Engine)
font files.

\subsection sfx_ss Special effects

A small set of screen effects are included for enhancing game presentation

- Fade out
- Flash 
- Quake (shake-cam)

\subsection memory_ss Memory management

BlueGin uses a consistent memory management model with reference counting for
allocations.  In most cases you will not need to delete entities yourself, nor
write destructor methods.

Internally, BlueGin uses the small object allocator from Box2D to speed up
large batches of allocations.


\section online_sec BlueGin online

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

