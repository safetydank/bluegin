# Cinder
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/../../BlueGin.mk

LOCAL_MODULE 	 		:= cinder
LOCAL_CFLAGS	 := $(BLUEGIN_CFLAGS)
LOCAL_C_INCLUDES := $(BLUEGIN_INCLUDE)

LOCAL_SRC_FILES  := src/gl/gl.cpp \
				    src/gl/Fbo.cpp \
				    src/gl/Light.cpp \
				    src/gl/Material.cpp \
					src/gl/Texture.cpp \
				    src/gl/Vbo.cpp \
					src/Area.cpp \
					src/AxisAlignedBox.cpp \
					src/BSpline.cpp \
					src/Camera.cpp \
					src/Color.cpp \
					src/Exception.cpp \
					src/Matrix.cpp \
					src/Path2D.cpp \
					src/Perlin.cpp \
					src/PolyLine.cpp \
					src/Rand.cpp \
					src/Rect.cpp \
					src/Shape2D.cpp \
					src/Sphere.cpp \
					src/TriMesh.cpp

include $(BUILD_STATIC_LIBRARY)

