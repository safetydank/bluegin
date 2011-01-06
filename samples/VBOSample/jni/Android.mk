#  OpenGL lighting example
#
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/../../../bluegin/BlueGin.mk

LOCAL_MODULE 	 := libvbosample
LOCAL_CFLAGS 	 := $(BLUEGIN_CFLAGS)
LOCAL_C_INCLUDES := $(BLUEGIN_INCLUDE) jni/include

LOCAL_SRC_FILES  := src/VBOSampleApp.cpp

LOCAL_LDLIBS 	 := $(BLUEGIN_LDLIBS)

include $(BUILD_SHARED_LIBRARY)


