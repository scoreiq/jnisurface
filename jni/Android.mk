LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := SurfaceTest
LOCAL_SRC_FILES := SurfaceTest.cpp

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
