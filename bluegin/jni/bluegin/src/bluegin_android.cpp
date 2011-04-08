#include "bluegin/bluegin.h"

//  Java method IDs
static jmethodID MID_LOAD_TEXTURE;
static jmethodID MID_LOAD_RESOURCE;

static jmethodID MID_KEYBOARD_TOGGLE;

static jmethodID MID_MUSIC_PLAY;
static jmethodID MID_MUSIC_STOP;
static jmethodID MID_MUSIC_IS_PLAYING;

static jmethodID MID_SOUND_INIT;
static jmethodID MID_SOUND_LOAD;
static jmethodID MID_SOUND_PLAY;
static jmethodID MID_SOUND_STOP;
static jmethodID MID_SOUND_SET_PITCH;
static jmethodID MID_SOUND_SET_VOLUME;
static jmethodID MID_SOUND_PAUSE;
static jmethodID MID_SOUND_RESUME;

static jmethodID MID_GET_DOCDIR;

jclass    jBlueGin;

void cache_method_ids(JNIEnv* env)
{
    jclass cls = env->FindClass("com/expb/bluegin/BlueGinAndroid");
    jBlueGin = static_cast<jclass>(env->NewGlobalRef(cls));
    env->DeleteLocalRef(cls);
    // XXX make sure to env->DeleteGlobalRef(jBlueGin) at cleanup

    MID_LOAD_TEXTURE = env->GetStaticMethodID(jBlueGin,
	       "load_texture",
	       "(Ljava/lang/String;[I)I");
    MID_LOAD_RESOURCE = env->GetStaticMethodID(jBlueGin,
	       "load_resource",
	       "(Ljava/lang/String;)[B");
    MID_KEYBOARD_TOGGLE = env->GetStaticMethodID(jBlueGin,
            "keyboard_toggle",
            "(Z)V");
    MID_MUSIC_PLAY = env->GetStaticMethodID(jBlueGin,
	       "music_play",
	       "(Ljava/lang/String;Z)V");
    MID_MUSIC_STOP = env->GetStaticMethodID(jBlueGin,
	       "music_stop",
	       "()V");
    MID_MUSIC_IS_PLAYING = env->GetStaticMethodID(jBlueGin,
	       "music_is_playing",
	       "()Z");
    MID_SOUND_INIT = env->GetStaticMethodID(jBlueGin,
            "sound_init",
            "()V");
    MID_SOUND_LOAD = env->GetStaticMethodID(jBlueGin,
            "sound_load",
            "(Ljava/lang/String;)I");
    MID_SOUND_PLAY = env->GetStaticMethodID(jBlueGin,
            "sound_play",
            "(IFFIIF)I");
    MID_SOUND_STOP = env->GetStaticMethodID(jBlueGin,
            "sound_stop",
            "(I)V");
    MID_SOUND_SET_PITCH = env->GetStaticMethodID(jBlueGin,
            "sound_set_pitch",
            "(IF)V");
    MID_SOUND_SET_VOLUME = env->GetStaticMethodID(jBlueGin,
            "sound_set_volume",
            "(IFF)V");
    MID_SOUND_PAUSE = env->GetStaticMethodID(jBlueGin,
            "sound_pause",
            "(I)V");
    MID_SOUND_RESUME = env->GetStaticMethodID(jBlueGin,
            "sound_resume",
            "(I)V");
    MID_GET_DOCDIR = env->GetStaticMethodID(jBlueGin,
            "get_docdir",
            "()Ljava/lang/String;");
}

int bluegin_load_texture(const char *s, int* width, int* height) 
{
    JNIEnv *env = get_jnienv();

    // there could be some exception handling happening here, but there isn't
    jint ret;

    //  Texture dimensions [w, h]
    jintArray dim = env->NewIntArray(2);
    jstring mystr = env->NewStringUTF(s);
    ret = env->CallStaticIntMethod(jBlueGin, MID_LOAD_TEXTURE, mystr, dim);

    //  Read the texture dimensions from modified dim array
    int ndim[2];
    env->GetIntArrayRegion(dim, 0, 2, ndim);
    *width = ndim[0]; *height = ndim[1];
    Log("bluegin_load_texture: received dims %d %d", *width, *height);
    
    return ret;
}

void* bluegin_load_resource(const char *s, int* size)
{
    JNIEnv *env = get_jnienv();

    jstring path = env->NewStringUTF(s);
    jobject resData = env->CallStaticObjectMethod(jBlueGin, MID_LOAD_RESOURCE, path);

    jbyteArray* resArr = reinterpret_cast<jbyteArray*>(&resData);
    jsize len = env->GetArrayLength(*resArr);
    if (len == 0) {
        Log("bluegin_load_resource: NULL resource %s", s);
        return 0;
    }

    *size = len;
    // Log("bluegin_load_resource: received byte array length %d", len);

    jbyte* ret = new jbyte[len+1];
    env->GetByteArrayRegion(*resArr, 0, len, ret);
    //  add NUL terminator
    ret[len] = '\0';
    return static_cast<void*>(ret);
}

void bluegin_keyboard_toggle(bool show)
{
    JNIEnv *env = get_jnienv();
    env->CallStaticVoidMethod(jBlueGin, MID_KEYBOARD_TOGGLE, (jboolean) show);
}

void bluegin_music_play(const char* fname, bool looping)
{
    JNIEnv *env = get_jnienv();
    jstring jfname  = env->NewStringUTF(fname);
    env->CallStaticVoidMethod(jBlueGin, MID_MUSIC_PLAY, jfname, (jboolean) looping);
}

void bluegin_music_stop()
{
    JNIEnv *env = get_jnienv();
    env->CallStaticVoidMethod(jBlueGin, MID_MUSIC_STOP);
}

bool bluegin_music_is_playing()
{
    JNIEnv *env = get_jnienv();
    jboolean playing = env->CallStaticBooleanMethod(jBlueGin, MID_MUSIC_IS_PLAYING);
    return playing;
}

void bluegin_sound_init()
{
    JNIEnv *env = get_jnienv();
    env->CallStaticVoidMethod(jBlueGin, MID_SOUND_INIT);
}

int bluegin_sound_load(const char* fname)
{
    JNIEnv *env = get_jnienv();
    jstring jfname  = env->NewStringUTF(fname);
    jint soundID = env->CallStaticIntMethod(jBlueGin, MID_SOUND_LOAD, jfname);
    return soundID;
}

int bluegin_sound_play(int sid, float leftVolume, float rightVolume, 
        int priority, int loop, float rate)
{
    JNIEnv *env = get_jnienv();
    jint streamID = env->CallStaticIntMethod(jBlueGin, MID_SOUND_PLAY, (jint) sid, 
            (jfloat) leftVolume, (jfloat) rightVolume, 
            (jint) priority, (jint) loop, (jfloat) rate);
    return streamID;
}

void bluegin_sound_stop(int streamID)
{
    JNIEnv *env = get_jnienv();
    env->CallStaticVoidMethod(jBlueGin, MID_SOUND_STOP, (jint) streamID); 
}

void bluegin_sound_set_pitch(int streamID, float rate)
{
    JNIEnv *env = get_jnienv();
    env->CallStaticVoidMethod(jBlueGin, MID_SOUND_SET_PITCH, 
            (jint) streamID, (jfloat) rate); 
}

void bluegin_sound_set_volume(int streamID, float leftVolume, float rightVolume)
{
    JNIEnv *env = get_jnienv();
    env->CallStaticVoidMethod(jBlueGin, MID_SOUND_SET_VOLUME, 
            (jint) streamID, (jfloat) leftVolume, (jfloat) rightVolume); 
}

void bluegin_sound_pause(int streamID)
{
    JNIEnv *env = get_jnienv();
    env->CallStaticVoidMethod(jBlueGin, MID_SOUND_PAUSE, (jint) streamID);
}

void bluegin_sound_resume(int streamID)
{
    JNIEnv *env = get_jnienv();
    env->CallStaticVoidMethod(jBlueGin, MID_SOUND_RESUME, (jint) streamID);
}

string bluegin_get_docdir()
{
    JNIEnv *env = get_jnienv();
    jobject ddobj = env->CallStaticObjectMethod(jBlueGin, MID_GET_DOCDIR);
    jstring* ddstr = reinterpret_cast<jstring*>(&ddobj);
    string docdir(env->GetStringUTFChars(*ddstr, 0));
    return docdir;
}

