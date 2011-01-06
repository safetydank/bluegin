#include "bluegin/bluegin.h"

#include <cstdio>

int bluegin_load_texture(const char *s, int* width, int* height) 
{
    return 0;
}

void* bluegin_load_resource(const char *assetPath, int* size)
{
    FILE* file;

    char path[256];
    _snprintf(path, 256, "assets/%s", assetPath);
    for (int i=0; i < 256; ++i) {
        //  replace path separators on Windows
        if (path[i] == '/') path[i] = '\\';
    }

    file = fopen(path, "rb");
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = new char[length+1];
    int readBytes = fread(buffer, 1, length, file);
    if (readBytes != length) {
        Log("Error reading from file %s size %d : %d bytes read\n", path, length, readBytes);
    }
    fclose(file);
    buffer[length] = '\0';

    *size = readBytes;
    return buffer;
}

void bluegin_music_play(const char* fname)
{
}

void bluegin_music_stop()
{
}

void bluegin_sound_init()
{
}

int bluegin_sound_load(const char* fname)
{
    return 0;
}

int bluegin_sound_play(int sid, float leftVolume, float rightVolume, 
        int priority, int loop, float rate)
{
    return 0;
}

void bluegin_sound_stop(int streamID)
{
}

void bluegin_sound_set_pitch(int streamID, float rate)
{
}

void bluegin_sound_set_volume(int streamID, float leftVolume, float rightVolume)
{
}

void bluegin_sound_pause(int streamID)
{
}

void bluegin_sound_resume(int streamID)
{
}


