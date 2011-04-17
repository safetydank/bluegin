#include <time.h>

#include "bluegin/bluegin.h"
#include "bluegin/android.h"
#include "bluegin/app.h"

// XXX is there a better way to get/retain the jni env
JNIEnv* _my_jnienv = 0;
void set_jnienv(JNIEnv* env) { _my_jnienv = env; }
JNIEnv* get_jnienv() 
{ 
    if (!_my_jnienv) {
        Log("Error: no jnienv set, check order of initialization");
    }
    return _my_jnienv; 
}

//  instantiated via BLUEGIN_APP macro
extern bluegin::BlueginApp* theApp__;
extern void nativeCreate();

//  begin time in milliseconds
long beginTime;

long getNanoTime()
{
    struct timespec clock;
    clock_gettime(CLOCK_MONOTONIC, &clock);
    return (1e9*clock.tv_sec + clock.tv_nsec) / 1e6;
}

JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_create(JNIEnv* env)
{
    set_jnienv(env);
    cache_method_ids(env);
    nativeCreate();
}

JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_setup(JNIEnv* env)
{
    theApp__->doSetup();
    struct timespec clock;
    clock_gettime(CLOCK_MONOTONIC, &clock);
    beginTime = (1e9*clock.tv_sec + clock.tv_nsec) / 1e6;
}

JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_update(JNIEnv* env)
{
    // Log("native update()");
    struct timespec clock;
    clock_gettime(CLOCK_MONOTONIC, &clock);
    int curTime = ((1e9*clock.tv_sec + clock.tv_nsec) / 1e6);
    int elapsed = curTime - beginTime;
    beginTime = curTime;
    // Log("elapsed: %d ms", elapsed);
    theApp__->doUpdate(elapsed);
}

JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_draw(JNIEnv* env)
{
    // Log("native draw()");
    theApp__->doDraw();
}

JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_cleanup(JNIEnv* env)
{
    delete(theApp__);   
    theApp__ = 0;       
}

JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_pause(JNIEnv* env)
{
    theApp__->doPause();
}

JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_resume(JNIEnv* env)
{
    if (theApp__)
        theApp__->doResume();
}

JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_setScreenSize(JNIEnv* env,
    jobject thiz, jint width, jint height)
{
    Log("Screen size set to %d x %d", width, height);
    theApp__->doResize(width, height);
}

//  Updates touch input vars from Android
JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_addTouchEvent(JNIEnv* env,
    jobject thiz, int eventType, float x, float y, float px, float py, int id)
{
    theApp__->addTouchEvent(eventType, x, y, px, py, id);
}

JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_addKeyEvent(JNIEnv* env,
    jobject thiz, bool keyDown, int unicode, int modifiers, int keycode)
{
    theApp__->addKeyEvent(keyDown, (char) (unicode & 0xff), modifiers, keycode);
}

JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_setTouches(JNIEnv* env)
{
    theApp__->doTouch();
}

JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_setKeys(JNIEnv* env)
{
    theApp__->doKeys();
}

JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_setAccelerometer(JNIEnv* env,
    jobject thiz, jfloat x, jfloat y, jfloat z)
{
    theApp__->doAccelerated(x, y, z);              
}

