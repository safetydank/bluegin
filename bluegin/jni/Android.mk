TOP_PATH := $(call my-dir)

include $(TOP_PATH)/Box2D/Android.mk
include $(TOP_PATH)/cinder/Android.mk
include $(TOP_PATH)/bluegin/Android.mk
include $(TOP_PATH)/flx/Android.mk

all: box2d cinder bluegin flx

.DEFAULT: all

