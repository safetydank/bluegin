LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cinder-prebuilt
LOCAL_SRC_FILES := obj/local/armeabi/libcinder.a
LOCAL_EXPORT_LDLIBS := -lGLESv1_CM -llog
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := box2d-prebuilt
LOCAL_SRC_FILES := obj/local/armeabi/libbox2d.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := bluegin-prebuilt
LOCAL_SRC_FILES := obj/local/armeabi/libbluegin.a
LOCAL_EXPORT_LDLIBS := -llog
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := flx-prebuilt
LOCAL_SRC_FILES := obj/local/armeabi/libflx.a
LOCAL_EXPORT_LDLIBS := -llog
include $(PREBUILT_STATIC_LIBRARY)

