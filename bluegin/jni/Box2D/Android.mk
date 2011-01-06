# Box 2D
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/../../BlueGin.mk

BOX2D_DIST = Box2D_v2.1.2
B2SRC 	   = $(BOX2D_DIST)/Box2D/Box2D

LOCAL_C_INCLUDES := $(STLPORT_INCLUDE) jni/Box2D/Box2D_v2.1.2/Box2D
LOCAL_MODULE	 := box2d
LOCAL_CFLAGS 	 := -Werror -DANDROID
LOCAL_SRC_FILES  := $(B2SRC)/Collision/Shapes/b2CircleShape.cpp \
					$(B2SRC)/Collision/Shapes/b2PolygonShape.cpp \
					$(B2SRC)/Collision/b2BroadPhase.cpp \
					$(B2SRC)/Collision/b2CollideCircle.cpp \
					$(B2SRC)/Collision/b2CollidePolygon.cpp \
					$(B2SRC)/Collision/b2Collision.cpp \
					$(B2SRC)/Collision/b2Distance.cpp \
					$(B2SRC)/Collision/b2DynamicTree.cpp \
					$(B2SRC)/Collision/b2TimeOfImpact.cpp \
					$(B2SRC)/Common/b2BlockAllocator.cpp \
					$(B2SRC)/Common/b2Math.cpp \
					$(B2SRC)/Common/b2Settings.cpp \
					$(B2SRC)/Common/b2StackAllocator.cpp \
					$(B2SRC)/Dynamics/Contacts/b2CircleContact.cpp \
					$(B2SRC)/Dynamics/Contacts/b2Contact.cpp \
					$(B2SRC)/Dynamics/Contacts/b2ContactSolver.cpp \
					$(B2SRC)/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
					$(B2SRC)/Dynamics/Contacts/b2PolygonContact.cpp \
					$(B2SRC)/Dynamics/Contacts/b2TOISolver.cpp \
					$(B2SRC)/Dynamics/Joints/b2DistanceJoint.cpp \
					$(B2SRC)/Dynamics/Joints/b2FrictionJoint.cpp \
					$(B2SRC)/Dynamics/Joints/b2GearJoint.cpp \
					$(B2SRC)/Dynamics/Joints/b2Joint.cpp \
					$(B2SRC)/Dynamics/Joints/b2LineJoint.cpp \
					$(B2SRC)/Dynamics/Joints/b2MouseJoint.cpp \
					$(B2SRC)/Dynamics/Joints/b2PrismaticJoint.cpp \
					$(B2SRC)/Dynamics/Joints/b2PulleyJoint.cpp \
					$(B2SRC)/Dynamics/Joints/b2RevoluteJoint.cpp \
					$(B2SRC)/Dynamics/Joints/b2WeldJoint.cpp \
					$(B2SRC)/Dynamics/b2Body.cpp \
					$(B2SRC)/Dynamics/b2ContactManager.cpp \
					$(B2SRC)/Dynamics/b2Fixture.cpp \
					$(B2SRC)/Dynamics/b2Island.cpp \
					$(B2SRC)/Dynamics/b2World.cpp \
					$(B2SRC)/Dynamics/b2WorldCallbacks.cpp

include $(BUILD_STATIC_LIBRARY)

