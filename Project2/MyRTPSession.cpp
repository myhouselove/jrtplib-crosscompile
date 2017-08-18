#include "MyRTPSession.h"



MyRTPSession::MyRTPSession()
{
#ifdef RTP_SOCKETTYPE_WINSOCK
    WSADATA dat;
	WSAStartup(MAKEWORD(2,2),&dat);
#endif // RTP_SOCKETTYPE_WINSOCK

    m_Sessparams.SetOwnTimestampUnit(1.0/90000.0);
    m_SessionCreated = false;
    m_OneFrameDataRecved = false;
}

void MyRTPSession::CheckError(int rtperr)
{
    if (rtperr < 0)
    {

        //std::cout << "ERROR: " << RTPGetErrorString(rtperr) << std::endl;
        //exit(-1);
        //PostQuitMessage(0);
    }
}

string MyRTPSession::GetRTPLibVersion()
{
    return RTPLibraryVersion::GetVersion().GetVersionString();
}

void MyRTPSession::SetLocalPort(int port)
{

    //m_Sessparams.SetUsePredefinedSSRC(true);  //设置使用预先定义的SSRC
    m_Sessparams.SetOwnTimestampUnit(1.0 / 10.0); /* 设置采样间隔 */

    //m_Sessparams.SetOwnTimestampUnit(1.0/10.0);

    m_LocalPort = port;
    m_Transparams.SetPortbase(m_LocalPort);   //监听本地端口

    int status = m_Sess.Create(m_Sessparams, &m_Transparams);
    CheckError(status);
};

void MyRTPSession::SetDestIpAndPort(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4,int port)
{
    m_DestPort = port;

    m_DestIp[0] = p1;
    m_DestIp[1] = p2;
    m_DestIp[2] = p3;
    m_DestIp[3] = p4;

    RTPIPv4Address addr(m_DestIp, m_DestPort);
    int status = m_Sess.AddDestination(addr);
    CheckError(status);

    m_Sess.SetDefaultTimestampIncrement(3600);/* 设置时间戳增加间隔 */
    m_Sess.SetDefaultPayloadType(96);
    m_Sess.SetDefaultMark(true);
};

void MyRTPSession::SendFragmentData(uint8_t *buffer,int index, int buflen)
{
    int status;
    uint8_t *pSendbuf;
    pSendbuf = buffer;
    if(buflen <= MAXLEN)  //每次读的是1024大小的数据
    {
        m_Sess.SetDefaultMark(true);
        status = m_Sess.SendPacket((void *)&pSendbuf[0],buflen);
        CheckError(status);
        //printf("send_packt index = %d len = %d\n",index, buflen);
    }
};

void MyRTPSession::SendFrameData(char* buffer, int length)
{
    int MAX_BUFFER_SIZE = 1024;
    int count = length / MAX_BUFFER_SIZE + 1;
    int index = 0;

    SendTotalCount(count, length);

    int sum = 0;
    for(int i = 0; i<count;i++)
    {
        if(i < count -1)
        {
            SendFragmentData((uint8_t*)(buffer + i * MAX_BUFFER_SIZE), index, MAX_BUFFER_SIZE);
        }
        else
        {
            SendFragmentData((uint8_t*)(buffer + i * MAX_BUFFER_SIZE), index, length - i * MAX_BUFFER_SIZE);
        }
        RTPTime::Wait(0.0001); //间隔10毫秒
    }
}

char* MyRTPSession::RecvData()
{
    return NULL;
};

void MyRTPSession::SendTotalCount(int totalCount, int length)
{
    char* cmd = new char[100];
    sprintf(cmd, "%s-%d-%d", "Start Sending First Frame", totalCount, length);
    cout<<"cmd = "<<cmd<<endl;
    SendFragmentData((uint8_t*)cmd, 0, 100);
    delete[] cmd;
};

int MyRTPSession::parseTotalCount(int strlength, char* str, int* totallength)
{
    char *buffer = new char[strlength];
    memcpy(buffer, str, strlength);
    char *tmp = NULL;
    tmp = strtok(buffer, "-");
    if(tmp != NULL && strcmp(tmp,"Start Sending First Frame") == 0)
    {
        //cout<<"Total count = "<<tmp<<endl;

        tmp = strtok(NULL, "-");
        int count = 0;
        if (tmp != NULL)
        {
            count = atoi(tmp);
            tmp = strtok(NULL, "-");
            if (tmp != NULL)
            {
                *totallength = atoi(tmp);
                delete[] buffer;
                return count;
            }
        }
    }
    delete[] buffer;
    *totallength = 0;
    return -1;
}



