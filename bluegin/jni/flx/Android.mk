# Flixel-based 2D sprite engine
#
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/../../BlueGin.mk

LOCAL_MODULE     	  := flx
LOCAL_CFLAGS	 := $(BLUEGIN_CFLAGS)
LOCAL_C_INCLUDES := $(BLUEGIN_INCLUDE)

LOCAL_SRC_FILES  := src/data/anim.cpp \
					src/data/fade.cpp \
					src/data/flash.cpp \
					src/data/list.cpp \
					src/data/quake.cpp \
					src/data/rect.cpp \
					src/data/touch.cpp \
					src/phy/pdebugdraw.cpp \
					src/phy/pgame.cpp \
					src/phy/psprite.cpp \
					src/phy/pworld.cpp \
					src/button.cpp \
					src/emitter.cpp \
					src/flx.cpp \
					src/flxG.cpp \
					src/game.cpp \
					src/group.cpp \
					src/object.cpp \
					src/quadtree.cpp \
					src/render.cpp \
					src/sound.cpp \
					src/sprite.cpp \
					src/state.cpp \
					src/text.cpp \
					src/tileblock.cpp \
					src/tilemap.cpp \
					src/FlxApp.cpp

include $(BUILD_STATIC_LIBRARY)

