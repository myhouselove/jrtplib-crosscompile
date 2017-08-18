LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= tools/gettypes.cpp \
                src/extratransmitters/rtpfaketransmitter.cpp \
                src/rtcpapppacket.cpp \
                src/rtcpbyepacket.cpp \
                src/rtcpcompoundpacket.cpp \
                src/rtcpcompoundpacketbuilder.cpp \
                src/rtcppacket.cpp \
                src/rtcppacketbuilder.cpp \
                src/rtcprrpacket.cpp \
                src/rtcpscheduler.cpp \
                src/rtcpsdesinfo.cpp \
                src/rtcpsdespacket.cpp \
                src/rtcpsrpacket.cpp \
                src/rtpcollisionlist.cpp \
                src/rtpdebug.cpp \
                src/rtperrors.cpp \
                src/rtpinternalsourcedata.cpp \
                src/rtpipv4address.cpp \
                src/rtpipv6address.cpp \
                src/rtplibraryversion.cpp \
                src/rtppacket.cpp \
                src/rtppacketbuilder.cpp \
                src/rtppollthread.cpp \
                src/rtprandom.cpp \
                src/rtpsession.cpp \
                src/rtpsessionparams.cpp \
                src/rtpsessionsources.cpp \
                src/rtpsourcedata.cpp \
                src/rtpsources.cpp \
                src/rtptimeutilities.cpp \
                src/rtpudpv4transmitter.cpp \
                src/rtpudpv6transmitter.cpp \
				src/jmutex.cpp \
				src/jthread.cpp \

LOCAL_CFLAGS += -DHAVE_TIME_H \
                -DHAVE_FCNTL_H \
                                -DHAVE_SYS_SELECT_H \
                                -DENABLE_TRACE \
                                -DHAVE_RTP_SUPPORT_IPV4MULTICAST\
                                -DOSIP_MT

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/include $(LOCAL_PATH)/src $(LOCAL_PATH)/tools #\     $(LOCAL_PATH)/../libjthread/include
LOCAL_EXPORT_C_INCLUDES:=$(LOCAL_C_INCLUDES)

LOCAL_STATIC_LIBRARIES :=\
           libjthread 

#LOCAL_LDLIBS += -lpthread -ljthread -ldl

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE:= libjrtp

include $(BUILD_STATIC_LIBRARY)
$(call import-module,libjthread)