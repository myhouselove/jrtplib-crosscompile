//
// Created by win7 on 2017/8/10 0010.
//
#include "UnityAPI.h"
#include <jni.h>
#include <string>
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , "AndroidAPI", __VA_ARGS__)

void callback(char* data,int len){

    LOGI("      data=%s,       len=%d \n",data,len);

}

extern "C" {
JNIEXPORT jstring JNICALL
Java_wmy_jni_com_jnijrtp_MainActivity_rtptransport(JNIEnv *env, jobject thiz) {
    LOGI("11111111111111111111111 Java_wmy_jni_com_jnijrtp_MainActivity_rtptransport");
    int a = UnityAPI_SetLocalPort(12346);
    int b = UnityAPI_SetDestIpAndPort(192,168,1,106,12348);
    //int c = UnityAPI_Release();
    int i = 20;
//    while(i-- != 0) {
//        LOGI("11111111111111111111111");
//
//        int d = UnityAPI_SendFrameData("12345678", 20);
//    }
    UnityAPI_SetRecvDataCallback(callback);
    UnityAPI_StartRecvDataThread();
    //char* e = UnityAPI_RecvData();

   // LOGI("11111111111111111111111a = %d,b=%d,c=%d,d=%d,e=%s\n",a,b,c,d,e);
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}



}