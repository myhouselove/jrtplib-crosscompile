#include <jni.h>
#include <pthread.h>
#ifdef __cplusplus

extern "C" {

void Java_com_rtptran_CMainRTP_rtptransport(JNIEnv *, jobject);

}

#endif
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

#include <android/log.h>
const char* const LOG_TAG = "RTP_JNI";
//using namespace jrtplib

//
// This function checks if there was a RTP error. If so, it displays an error
// message and exists.
//
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO  , "AndroidAPI", __VA_ARGS__)
void checkerror(jint rtperr)
{
	if (rtperr < 0)
	{
        LOGI("THE WRONG IS = %s\n",RTPGetErrorString(rtperr).c_str());
		std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
		exit(-1);
	}
}

//
// The main routine
//
MyRTPSession myRTPSession;
#define MAX_BUFFER_SIZE 1024

void *sendthread_func(void *arg){
    char* buffer = new char[20];
    buffer = "123456789";
int i = 20;
    LOGI("1111111111");
while(i-- != 0) {
    LOGI("22222222222");
    myRTPSession.SendFrameData(buffer, 20);

}
    return NULL;
}


void *recvthread_func(void *arg){
    LOGI("Start Recv Thread.....\n");
    int totalCount = 0;
    int index = 0;
    char* buffer = NULL;
    bool firstFrame = true;
    int res = 0;

    bool stop = false;
    while(!stop)
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
                    LOGI("LEN IS = %d         data = %s\n",len,loaddata);
//                    if(res == 0)
//                    {
//                    	res = myRTPSession.parseTotalCount(pack ->GetPayloadLength(), loaddata);
//                    }
//
//                    if(res != -1 && buffer == NULL)
//                    {
//                    	totalCount = res;
//                    	//printf("TotalCount = %d\n", totalCount);
//                    	buffer = (char*)malloc(sizeof(char) * totalCount * MAX_BUFFER_SIZE);
//                    }
//                    else
//                    {
//                    	//printf("Index = %d  len = %d\n",index, len);
//
//                    	memcpy(buffer + index * MAX_BUFFER_SIZE, loaddata, len);
//                    	index++;
//                    	if(index == totalCount)
//                    	{
//                    		cout<<"start write to jpg  TotalLength = "<<(index - 1) * MAX_BUFFER_SIZE + len<<endl;
//                    		//write to file
//                    		char* fileName = new char[20];
//                    		sprintf(fileName, "%d.jpg", fileIndex++);
//                    		FILE* file = fopen(fileName , "wb");
//                    		fwrite(buffer, (index - 1) * MAX_BUFFER_SIZE + len, 1, file);
//                    		fclose(file);
//                    		cout<<"end write to jpg"<<endl;
//                    		index = 0;
//                    		totalCount = 0;
//                    		res = 0;
//                    		delete[] buffer;
//                    		buffer = NULL;
//                    	}
//                    }
                    myRTPSession.m_Sess.DeletePacket(pack);
                }
            } while (myRTPSession.m_Sess.GotoNextSourceWithData());
        }

        myRTPSession.m_Sess.EndDataAccess();
    }

}

extern "C" {
JNIEXPORT jstring JNICALL
Java_wmy_jni_com_jnijrtp_MainActivity_rtptransport(JNIEnv *env, jobject thiz) {
    pthread_t sendthread;
    pthread_t recvthread;

    myRTPSession.SetLocalPort(12348);

    myRTPSession.SetDestIpAndPort(192, 168, 1, 106, 10000);

    pthread_create(&sendthread, NULL, sendthread_func, NULL);
    pthread_create(&recvthread, NULL, recvthread_func, NULL);

    LOGI("22222222222");
    // pthread_join(fitthread,NULL);
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

JNIEXPORT jstring JNICALL
Java_wmy_jni_com_jnijrtp_MainActivity_rtptransport1(JNIEnv *env, jobject thiz) {


    LOGI("1111111111111111111111\n");

    RTPSession sess;
    uint16_t portbase, destport;
    uint32_t destip;
    std::string ipstr;
    int status, i, num;

    // First, we'll ask for the necessary information

//	std::cout << "Enter local portbase:" << std::endl;
//	std::cin >> portbase;
//	std::cout << std::endl;
    portbase = 12348;

//	std::cout << "Enter the destination IP address" << std::endl;
//	std::cin >> ipstr;
    ipstr = "192.168.1.106";

    destip = inet_addr(ipstr.c_str());
    if (destip == INADDR_NONE) {
        //	std::cerr << "Bad IP address specified" << std::endl;
        std::string hello = "Hello from C++";
        return env->NewStringUTF(hello.c_str());
    }

    // The inet_addr function returns a value in network byte order, but
    // we need the IP address in host byte order, so we use a call to
    // ntohl
    destip = ntohl(destip);

//	std::cout << "Enter the destination port" << std::endl;
//	std::cin >> destport;
    destport = 10000;

    //std::cout << std::endl;
//	std::cout << "Number of packets you wish to be sent:" << std::endl;
//	std::cin >> num;
    num = 500;

    // Now, we'll create a RTP session, set the destination, send some
    // packets and poll for incoming data.


//jboolean bl = (*env)->CallBooleanMethod(env, thiz, mid, js);

    RTPUDPv4TransmissionParams transparams;
    RTPSessionParams sessparams;

    // IMPORTANT: The local timestamp unit MUST be set, otherwise
    //            RTCP Sender Report info will be calculated wrong
    // In this case, we'll be sending 10 samples each second, so we'll
    // put the timestamp unit to (1.0/10.0)
    sessparams.SetOwnTimestampUnit(1.0 / 10.0);

    sessparams.SetAcceptOwnPackets(true);
    transparams.SetPortbase(portbase);
    status = sess.Create(sessparams, &transparams);
    checkerror(status);

    RTPIPv4Address addr(destip, destport);

    status = sess.AddDestination(addr);
//	checkerror(status);

    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s", "ready\n");


    for (i = 1; i <= num; i++) {
        LOGI("\nSending packet %d/%d\n", i, num);

        // send the packet
        status = sess.SendPacket((void *) "1234567890", 10, 0, false, 10);
        //checkerror(status);
        //	 jboolean bl = (*env)->CallBooleanMethod(env, thiz, regsec, jregsec);

        sess.BeginDataAccess();

        // check incoming packets
        if (sess.GotoFirstSourceWithData()) {
            do {
                RTPPacket *pack;

                while ((pack = sess.GetNextPacket()) != NULL) {
                    // You can examine the data here
                    //	printf("Got packet !\n");

                    // we don't longer need the packet, so
                    // we'll delete it
                    sess.DeletePacket(pack);
                }
            } while (sess.GotoNextSourceWithData());
        }

        sess.EndDataAccess();

#ifndef RTP_SUPPORT_THREAD
        status = sess.Poll();
//		checkerror(status);
#endif // RTP_SUPPORT_THREAD

        RTPTime::Wait(RTPTime(1, 0));
    }

    sess.BYEDestroy(RTPTime(10, 0), 0, 0);

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


}





