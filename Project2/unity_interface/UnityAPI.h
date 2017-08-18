//
// Created by win7 on 2017/8/10 0010.
//

#ifndef JNIJRTP_UNITYAPI_H
#define JNIJRTP_UNITYAPI_H

#include <stdint.h>

#if 0
#define EXPORT_DLL __declspec(dllexport) //导出dll声明
#else
#define EXPORT_DLL
#endif

extern "C" {
EXPORT_DLL int MyAddFunc(int _a, int _b);

EXPORT_DLL int UnityAPI_SetLocalPort(int localPort);
EXPORT_DLL int UnityAPI_SetDestIpAndPort(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4,int port);
EXPORT_DLL int UnityAPI_Release();
EXPORT_DLL int UnityAPI_SendFrameData(char* buffer, int length);
EXPORT_DLL int UnityAPI_StartRecvDataThread();
EXPORT_DLL void UnityAPI_GetRecvData(void *p_data);
/*
 *
 * 设置回调 回调给上层char* buffer和数据的长度
 * */
EXPORT_DLL int UnityAPI_SetRecvDataCallback(void (*callback)(char* data,int len));


}



#endif //JNIJRTP_UNITYAPI_H
