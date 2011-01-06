# Bluegin library
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/../../BlueGin.mk

LOCAL_MODULE 	 := bluegin
LOCAL_CFLAGS	 := $(BLUEGIN_CFLAGS)
LOCAL_C_INCLUDES := $(BLUEGIN_INCLUDE)

LOCAL_SRC_FILES  := src/bluegin_jni.cpp \
					src/bluegin_android.cpp \
					src/bluegin_common.cpp \
					src/app.cpp \
					src/config.cpp \
					src/graphic.cpp \
					src/hgefont.cpp \
					src/resourcemanager.cpp \
					src/sound.cpp 

LOCAL_STATIC_LIBRARIES := libcinder libflx libBox2D
LOCAL_LDLIBS 	 := -lGLESv1_CM -llog -ldl 

include $(BUILD_STATIC_LIBRARY)

