LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

UNITY_LIB := $(LOCAL_PATH)/../../jniLibs/armeabi-v7a




LOCAL_CFLAGS +=   -DENABLE_TRACE


LOCAL_EXPORT_C_INCLUDES:=$(LOCAL_C_INCLUDES)
LOCAL_C_INCLUDES += $(LOCAL_PATH)

LOCAL_LDLIBS += -lrtp_jni
LOCAL_LDLIBS += -L$(UNITY_LIB)
LOCAL_LDLIBS += -llog

LOCAL_SRC_FILES := test_android.cpp

LOCAL_MODULE    := testandroid
include $(BUILD_SHARED_LIBRARY)

