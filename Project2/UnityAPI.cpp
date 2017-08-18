//
// Created by win7 on 2017/8/10 0010.
//
#include <jni.h>
#include <pthread.h>
#include "rtppacket.h"
#include "rtpsession.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#ifndef WIN32
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#endif // WIN32
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "MyRTPSession.h"
#include<vector>
#include <android/log.h>

#include "UnityAPI.h"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , "AndroidAPI", __VA_ARGS__)

extern "C" {
MyRTPSession myRTPSession;
bool recv_thread_flag = false;
pthread_t recvthread;
RecvCallback  OnRecvDataCallback;
bool g_set_callback_ok_flag = false;
int MAX_BUFFER_SIZE = 1024;
int fileIndex = 0;
char *f_unity_buffer = NULL;
pthread_mutex_t lockbuffer;

int MAX_VECTOR_BUFFER_SIZE = 3;
vector<char *> g_RecvDataVector(MAX_VECTOR_BUFFER_SIZE);
vector<int>    g_RecvDataLength(MAX_VECTOR_BUFFER_SIZE);
int vectorIndex = 0;


void *recvthread_func(void *arg){
    LOGI("Start Recv Thread.....\n");

    int totalCount = 0;
    int index = 0;
    char* buffer = NULL;
    bool firstFrame = true;
    int res = 0;
    int totallength = 0;
    //pthread_mutex_init(&lockbuffer, NULL);

    for(int i = 0;i<MAX_VECTOR_BUFFER_SIZE;i++) {
        g_RecvDataVector.at(i) = NULL;
    }

    while(!recv_thread_flag)
    {
        RTPPacket *pack;
        int	status = myRTPSession.m_Sess.Poll();  // 主动收包
        myRTPSession.m_Sess.BeginDataAccess();
        if (myRTPSession.m_Sess.GotoFirstSourceWithData())
        {
            do
            {
                while ((pack = myRTPSession.m_Sess.GetNextPacket()) != NULL)
                {
                    char* loaddata = (char*)pack->GetPayloadData();
                    size_t len         = pack->GetPayloadLength();
                    // TRACE(" Get packet-> %d  \n ",pack->GetPayloadLength());
                    //LOGI("len ---------------=%d\n",len);
                    //if(res == 0)
                    //{
                    int tmpLength = 0;
                    res = myRTPSession.parseTotalCount(pack ->GetPayloadLength(), loaddata, &tmpLength);
                    //}
                   // cout<<res<<endl;
                    if(res != -1 )
                    {
                        totallength = tmpLength;
                        totalCount = res;
                        index = 0;
                        //printf("TotalCount = %d\n", totalCount);
                       // LOGI("malloc buffer totalCount---------------=%d    length = %d\n",totalCount, totallength);
                        if (buffer != NULL)
                        {
                            free(buffer);
                            buffer = NULL;
                        }
                        buffer = (char*)malloc(sizeof(char) * totalCount * 1024);

                    }
                    else
                    {
                        //printf("Index = %d  len = %d\n",index, len);

                        if(buffer != NULL)
                        {


                            memcpy(buffer + index * MAX_BUFFER_SIZE, loaddata, len);

                            index++;
                            if(index == totalCount)
                            {
                                //LOGI("memcpy buffer len---------------=%d\n",len);
                                //LOGI("=====================  end %d",vectorIndex);
                               // cout<<"start write to jpg  TotalLength = "<<(index - 1) * MAX_BUFFER_SIZE + len<<endl;
                                //write to file
                                //pthread_mutex_lock(&lockbuffer);
                                if (vectorIndex <= MAX_VECTOR_BUFFER_SIZE - 1)
                                {
                                    if (g_RecvDataVector.at(vectorIndex) != NULL)
                                    {
                                        free(g_RecvDataVector.at(vectorIndex));
                                        g_RecvDataVector.at(vectorIndex) = NULL;
                                    }

                                    g_RecvDataLength.at(vectorIndex) = totallength;
                                    g_RecvDataVector.at(vectorIndex) = (char *) malloc(totallength);
                                   // LOGI("11111111111111111  %d", totallength);
                                    memcpy(g_RecvDataVector.at(vectorIndex),buffer,totallength );
                                    vectorIndex++;
                                    if (vectorIndex == MAX_VECTOR_BUFFER_SIZE){
                                        vectorIndex = 0;
                                    }
                                }



//                                if (f_unity_buffer == NULL) {
//                                    f_unity_buffer = (char *) malloc(sizeof(char) * totalCount * 1024);
//                                }
//                                memcpy(f_unity_buffer,buffer,totallength);
//                                pthread_mutex_unlock(&lockbuffer);
//                                OnRecvDataCallback(totallength);
                                totallength = 0;
//                                char* fileName = new char[20];
//                                sprintf(fileName, "/sdcard/screenshots/%d.jpg", fileIndex++);
//                                FILE* file = fopen(fileName , "wb");
//                                fwrite(buffer, totallength, 1, file);
//                                fclose(file);
//                                LOGI("end write to jpg");
                                index = 0;
                                totalCount = 0;
                                res = 0;
                                free(buffer);
                                buffer = NULL;
                            }
                        }


                    }
                    myRTPSession.m_Sess.DeletePacket(pack);
                }
            } while (myRTPSession.m_Sess.GotoNextSourceWithData());
        }

        myRTPSession.m_Sess.EndDataAccess();
    }

}


int MyAddFunc(int _a, int _b)
{
    return _a + _b;
}
/*
 * 设置本地端口
 *
 * */


int UnityAPI_SetLocalPort(int localPort){


    myRTPSession.SetLocalPort(localPort);
    return 1;
}
/*
 *
 *设置目的端口和目的IP
 * */
int UnityAPI_SetDestIpAndPort(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4,int port){

    myRTPSession.SetDestIpAndPort(p1,p2, p3,p4,port);
    return 2;
}
/*
 *release 资源
 *
 * */
int UnityAPI_Release(){
    recv_thread_flag = true;
    pthread_join(recvthread,NULL);
    return 0;
}
/*
 *发送数据
 *
 * */
int UnityAPI_SendFrameData(char* buffer, int length){
    myRTPSession.SendFrameData(buffer,length);
    return 4;
}
/*
 *启动接收线程
 *
 * */

int UnityAPI_StartRecvDataThread(){
    //if (g_set_callback_ok_flag)
    {
        recv_thread_flag = false;
        pthread_create(&recvthread, NULL, recvthread_func, NULL);
        return 0;
    }
    return -1;
}
/*
 *收数据j
 *
 * */
void UnityAPI_GetRecvData(void *p_data,int len){
//    pthread_mutex_lock(&lockbuffer);
//    memcpy(p_data,f_unity_buffer,len);
//    pthread_mutex_unlock(&lockbuffer);
//    free(f_unity_buffer);

    int index = (vectorIndex + MAX_VECTOR_BUFFER_SIZE / 2) % MAX_VECTOR_BUFFER_SIZE;
    //LOGI("================================= %d", index);
    if (g_RecvDataVector.at(index) == NULL)
    {
        LOGI("================================= NULL");
    }
    if (index <= MAX_BUFFER_SIZE - 1 && g_RecvDataVector.at(index) != NULL)
    {
        //LOGI("Memory copy size = %d", strlen(g_RecvDataVector.at(index)));
        memcpy(p_data, g_RecvDataVector.at(index), g_RecvDataLength.at(index));
       // p_data = (g_RecvDataVector.at(index));
    }

}




   // return f_unity_buffer;

}
/*
 *
 * 设置回调
 * */
int UnityAPI_SetRecvDataCallback(RecvCallback callback){
    OnRecvDataCallback = callback;
    if (OnRecvDataCallback != NULL){
        g_set_callback_ok_flag = true;
        return 0;
    }
    return -1;
}

