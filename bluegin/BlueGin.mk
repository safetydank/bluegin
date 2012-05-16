# Rules for linking against native libraries supplied by bluegin
#
NATIVE_PATH := $(call my-dir)

# Read Boost settings
include $(NATIVE_PATH)/Common.mk

BLUEGIN_INCLUDE := $(BOOST_INCLUDE) $(NATIVE_PATH)/jni/cinder/include $(NATIVE_PATH)/jni/bluegin/include $(NATIVE_PATH)/jni/flx/include $(NATIVE_PATH)/jni/Box2D/Box2D_v2.1.2/Box2D $(NATIVE_PATH)/jni/rapidxml/rapidxml-1.13
BLUEGIN_CFLAGS  := -DANDROID $(BOOST_CFLAGS) -DBLUEGIN -DCINDER_BLUEGIN -DRAPIDXML_NO_EXCEPTIONS -Werror -D__LITTLE_ENDIAN__ -D_GLIBCPP_USE_WCHAR_T
BLUEGIN_LDLIBS  := -L$(NATIVE_PATH)/obj/local/armeabi -lcinder -lflx -lbluegin -lbox2d -lGLESv1_CM -llog -ldl -lgcc

