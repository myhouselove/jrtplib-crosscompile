#ifndef MY_RTPSESSION_INCLUDE
#define MY_RTPSESSION_INCLUDE

#include "rtpsession.h"
#include "rtpudpv4transmitter.h"
#include "rtpipv4address.h"
#include "rtpsessionparams.h"
#include "rtperrors.h"
#include "rtplibraryversion.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;



#define MAXLEN (RTP_DEFAULTPACKETSIZE - 10)   //默认值为1400


class MyRTPSession : public RTPSession
{
public:
    MyRTPSession();
    ~MyRTPSession(){};

    void CheckError(int rtperr);

    void SetDestIpAndPort(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4,int port);
    void SetLocalPort(int port);
    void SendFrameData(char* buffer, int length);
    void SendTotalCount(int totalCount, int length);
    void SendFragmentData(uint8_t *h264buf,int index, int buflen);
    char* RecvData();
    string GetRTPLibVersion();


    int parseTotalCount(int strlength, char* str, int* totallength);

public:
    RTPSession m_Sess;
    RTPUDPv4TransmissionParams m_Transparams;
    RTPSessionParams m_Sessparams;


    int m_DestPort;
    int m_LocalPort;
    uint8_t m_DestIp[4];
    bool m_SessionCreated;

    bool m_OneFrameDataRecved;

};

#endif