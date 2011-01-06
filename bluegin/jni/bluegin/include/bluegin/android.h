#pragma once

#include <jni.h>

#include <android/log.h>
#define Log(...) __android_log_print(ANDROID_LOG_DEBUG,"BlueGin",__VA_ARGS__)

JNIEnv* get_jnienv();

void cache_method_ids(JNIEnv* env);

extern "C" {
    //  Native functions called by Java
    JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_create(JNIEnv* env);
    JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_setup(JNIEnv* env);
    JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_update(JNIEnv* env);
    JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_draw(JNIEnv* env);
    JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_addTouchEvent(JNIEnv* env,
        jobject thiz, int eventType, float x, float y, float px, float py, int id);
    JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_setTouches(JNIEnv* env);
    JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_setAccelerometer(JNIEnv* env,
        jobject thiz, jfloat x, jfloat y, jfloat z);
    JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_setScreenSize(JNIEnv* env,
        jobject thiz, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_cleanup(JNIEnv* env);
    JNIEXPORT void JNICALL Java_com_expb_bluegin_Native_pause(JNIEnv* env);
};

