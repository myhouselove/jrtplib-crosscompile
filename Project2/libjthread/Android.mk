LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
                src/jmutex.cpp \
                src/jthread.cpp \
 
LOCAL_CFLAGS +=  -DHAVE_PTHREAD \
                 -DENABLE_TRACE \
                             

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/include $(LOCAL_PATH)/src 
LOCAL_EXPORT_C_INCLUDES:=$(LOCAL_C_INCLUDES)

LOCAL_PRELINK_MODULE := false
LOCAL_LDLIBS +=   -ldl -llog
 

LOCAL_MODULE:= libjthread

include $(BUILD_STATIC_LIBRARY)