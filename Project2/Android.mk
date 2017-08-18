LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := rtp_jni
LOCAL_SRC_FILES := 
#LOCAL_SRC_FILES := test_android.cpp


LOCAL_CFLAGS +=   -DENABLE_TRACE

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/jthread $(LOCAL_PATH)/jrtp
#LOCAL_C_INCLUDES += MyRTPSession.h
LOCAL_EXPORT_C_INCLUDES:=$(LOCAL_C_INCLUDES)


 
LOCAL_STATIC_LIBRARIES :=\
           libjthread libjrtp 

LOCAL_LDLIBS += -llog

include $(BUILD_SHARED_LIBRARY)
$(call import-add-path,$(LOCAL_PATH))

$(call import-module,libjthread)
$(call import-module,libjrtp)