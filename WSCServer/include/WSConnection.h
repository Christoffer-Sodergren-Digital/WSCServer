#ifndef WSCONNECTION_H
#define WSCONNECTION_H

#include <string>
#include "WSFrame.h"
#include "TCPSocket.h"

namespace WS{

enum WebSocketFrameType {
	TEXT_FRAME=0x01,
	BINARY_FRAME=0x02,
    CLOSE_FRAME=0x08,

	PING_FRAME=0x09,
	PONG_FRAME=0x0A
};

enum BinaryFrameType{
        GET_RESOURCE=0x01,
        GET_RESOURCEBINDINGS=0x02,
};

class WSConnection : public TCPSocket
{
    public:
        WSConnection();
        virtual ~WSConnection();

        inline bool Shutdown() const {return m_bShutdown;}
		WSFrame *ParseData(char *p_pData);
        WSFrame *ReceiveFrame();
    protected:
    private:

        void GenerateHandshake(const std::string & p_sData);
        void SendHTTPHandshakeResponse(const std::string & p_sKey);
        void ParseContent(WS::WSFrame *p_pxFrame);
        unsigned char *ParseFrame(char *p_pData,unsigned char *p_iOpCode);
        void ParseBinaryFrame(const char *p_pucFrameContent);
        unsigned char *CreateFrame(WebSocketFrameType p_eFrameType,const char *p_sMsg, int *p_iFrameSize);
        std::string Base64It(std::string p_sKey);
        SOCKET m_iSocket;
        std::string m_sHost;
        std::string m_sMagicNumber;
        int m_iVersion;
        bool m_bReady;
        bool m_bShutdown;
        time_t m_xLastTime;
};

}
#endif // WSCONNECTION_H
