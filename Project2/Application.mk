APP_STL:= stlport_static
APP_PLATFORM := android-19
#APP_MODULES := rtp_jni jthread  jrtp

APP_CFLAGS += -Wno-error=format-security
APP_BUILD_SCRIPT := Android.mk
APP_ABI := armeabi-v7a