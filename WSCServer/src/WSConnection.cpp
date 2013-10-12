#include "WSConnection.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <exception>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include "hl_sha1wrapper.h"
#include "base64.h"
#include "ResourceManager.h"
#include "WSFrame.h"
#include "CagerWorld.h"

using namespace std;
using namespace boost::algorithm;

namespace WS{

WSConnection::WSConnection() : m_bReady(false),m_bShutdown(false){
    m_xLastTime=time(NULL);
    m_sMagicNumber="258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
}
WSFrame *WSConnection::ReceiveFrame(){
     char sBuffer[2048];
    int iBR=this->Recv(sBuffer,2048,false);
    sBuffer[iBR]='\0';
    return ParseData(sBuffer);
}
WSFrame *WSConnection::ParseData(char *p_pData){
    if(p_pData==NULL){return NULL;}
    std::string sData(p_pData);
    if(!m_bReady){GenerateHandshake(sData);return NULL;}
    WS::WSFrame *pxFrame=new WS::WSFrame(p_pData);
    if(!pxFrame->IsValid()){return NULL;}
	return pxFrame;
}
void WSConnection::ParseContent(WS::WSFrame *p_pxFrame){
    unsigned char ucOC=p_pxFrame->OpCode();
    if(ucOC==TEXT_FRAME){
      /*  std::cout<<"Recieved text frame: "<<p_pxFrame->ApplicationData()<<"\n";
        CagerWorld *pxWorld=CagerWorld::Instance();

        std::ostringstream xS;
        IPAddr4 xAddr=GetRemoteAddr();
        xS<<"("<<xAddr<<")"<<(const char*)p_pxFrame->ApplicationData();
        std::string sMsg=xS.str();
        pxWorld->BroadcastMsg(sMsg,this);*/
    }else if(ucOC==BINARY_FRAME){
        try{
            ParseBinaryFrame((const char*)p_pxFrame->ApplicationData());
        }catch(unsigned char *p_pucError){
            std::cerr<<p_pucError<<"\n";
        }
    }
}
void WSConnection::ParseBinaryFrame(const char *p_pucFrameContent){
//    1 byte (opcode)
//    1 byte content len (0-125=0-125, 126=16bit, 127=64bit)
//    len byte (content)

    const char *pucC=p_pucFrameContent;
    int iL=strlen(pucC);
    unsigned char ucOpcode=(pucC[0] & 0xFF);
    ResourceManager *pxRM=ResourceManager::Instance();
    switch (ucOpcode){
        case GET_RESOURCE:{
                unsigned char ucResourceID=(pucC[1]&0xff);

                std::string sResource=pxRM->GetResourceByKey((int)ucResourceID);
                if(sResource!=RES_NOVAL){
                    int iFrameSize=0;
                    char *sContent=pxRM->ReadResourceFile(sResource);
                    if(sContent==NULL){
                        throw "Invalid file";
                    }
                   WSFrame *pxFrame=new WSFrame();
                   pxFrame->MakeFrame((const unsigned char*)sContent,OP_BINARY);
                    Send(pxFrame->Data(),pxFrame->FrameSize());
                }
            }
            break;
        case GET_RESOURCEBINDINGS:
                int iFrameSize=0;
                const std::string sMsg=pxRM->ResourceMapAsString();
                //unsigned char *pFrame=CreateFrame(TEXT_FRAME,sMsg.c_str(),&iFrameSize);
                WSFrame *pxFrame=new WSFrame();
                pxFrame->MakeFrame((const unsigned char*)sMsg.c_str(),OP_BINARY);
                Send(pxFrame->Data(),pxFrame->FrameSize());
//                const char *sFrame=reinterpret_cast<const char*>(pFrame);
//                send(m_iSocket,sFrame,iFrameSize,0);
            break;
    }


}
unsigned char *WSConnection::ParseFrame(char *p_pData, unsigned char *p_ucOpCode){
    int iInLen=strlen(p_pData);
    if(iInLen<3){m_bShutdown=true;return NULL;}

    unsigned char ucOpCode=(p_pData[0] & 0x0F);
    unsigned char ucFin=(p_pData[0] >> 7) & 0x01;
    unsigned char ucMasked=(p_pData[1]>>7) & 0x01;

    int iPayloadLen=0;
    int i=2;
    int iLenField=0; //manual bit manip, oh well
    for(int j=6,n=0;j>=0;j--,n++){
        bool bSet=(p_pData[1] & (1<<j));
        iLenField+=(bSet)?(pow(2,j)):0;
    }

    unsigned int uiMask=0;

    if(iLenField<=125){
        iPayloadLen=iLenField;
    }else if(iLenField==126){
        iPayloadLen=p_pData[2]+(p_pData[3]<<8);
        i+=2;
    }else if(iLenField==127){
        iPayloadLen=p_pData[2]+(p_pData[3]<<8);
        i+=8;
    }

    if(iInLen<iPayloadLen+i){
        m_bShutdown=true;
        return NULL;
    }
    if(ucMasked){
        uiMask=*((unsigned int*)(p_pData+i));
        i+=4;
        unsigned char *ucBuff=(unsigned char*)(p_pData+i);
        int iIdx,iC=iPayloadLen;
        for(iIdx=0;iIdx<iC;iIdx++){
            ucBuff[iIdx]=ucBuff[iIdx]^((unsigned char*)(&uiMask))[iIdx%4];
        }
    }
    unsigned char *sOutput=new unsigned char[iPayloadLen+1];
    memcpy(sOutput, p_pData+i, iPayloadLen);
    sOutput[iPayloadLen]='\0';
    if(ucOpCode==0x8){m_bShutdown=true;}

    *p_ucOpCode=ucOpCode;
    return sOutput;
}
void WSConnection::SendHTTPHandshakeResponse(const std::string & p_sKey){
    std::string sResponse="HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: ";
    sResponse+=p_sKey;
    sResponse+="\r\n\r\n";
    size_t iBytesSent=0;
    iBytesSent=Send(sResponse.c_str(),sResponse.length());
}
unsigned char *WSConnection::CreateFrame(WebSocketFrameType p_eFrameType, const char *p_sMsg, int *p_iFrameSize){
    int iPos=0;
    int iSize=strlen(p_sMsg);//.length();
    int iBuffSize=0;
    int iOffset=0;
	unsigned char *pBuffer=NULL;

    if(iSize<=125){
        pBuffer=new unsigned char[iSize+2];
        unsigned char ucWsFrame=0x08;
        ucWsFrame=((ucWsFrame<<4) | (p_eFrameType & 0x0f));
        pBuffer[iOffset++]=ucWsFrame;
        ucWsFrame=0;
        ucWsFrame=((ucWsFrame<<1) | iSize);
        pBuffer[iOffset++]=ucWsFrame;
    }else if(iSize<=65535){
        pBuffer=new unsigned char[iSize+4];
        pBuffer[iOffset++]=(unsigned char)p_eFrameType;
        pBuffer[iOffset++]=126;
        pBuffer[iOffset++]=(iSize>>8) & 0xFF;
        pBuffer[iOffset++]=iSize & 0xFF;
    }else{
        //todo add this
    }

    memcpy((void*)(pBuffer+iOffset),p_sMsg,iSize);
    *p_iFrameSize=iSize+iOffset;
    return pBuffer;
}
void WSConnection::GenerateHandshake(const std::string & p_sData){
    size_t iPos=p_sData.find("Host:");
    if(iPos==std::string::npos){m_bShutdown=true;return;}
    std::string sHost=p_sData.substr(iPos+5,p_sData.find("\r\n",iPos+6));
    sHost=sHost;
    m_bShutdown=true;
    if(sHost.length()<=0){return;}
    m_sHost=sHost;

    std::string sToken;
    iPos=p_sData.find("Upgrade: ");
    if(iPos==std::string::npos){return;}
    sToken=p_sData.substr(iPos+8,p_sData.find("\r\n",iPos+8)-(iPos+8));
    trim(sToken);
	std::transform(sToken.begin(), sToken.end(), sToken.begin(), ::tolower);
    if(sToken.find("websocket")==sToken.npos){return;}

    iPos=p_sData.find("Connection: ");
    if(iPos==std::string::npos){return;}
    sToken=p_sData.substr(iPos+12,p_sData.find("\r\n",iPos+12)-(iPos+12));
    trim(sToken);
	std::transform(sToken.begin(), sToken.end(), sToken.begin(), ::tolower);
	if(sToken.find("upgrade")==sToken.npos){return;}

    std::string sSearch="Sec-WebSocket-Key: ";
    iPos=p_sData.find(sSearch);
    if(iPos==std::string::npos){return;}
    size_t iEnd=p_sData.find("\r\n",iPos+sSearch.length());
    std::string sKey=p_sData.substr(iPos+sSearch.length(),iEnd-(iPos+sSearch.length()));
    trim(sKey);
    sKey+=m_sMagicNumber;
    sha1wrapper sha1;
    sKey=sha1.getHashFromString(sKey);

    //perform hex2string magic
    static const char* const lut = "0123456789ABCDEF";
    size_t len = sKey.length();
    if (len & 1){m_bShutdown=true;return;}

    std::string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
        char a = toupper(sKey[i]);
        const char* p = std::lower_bound(lut, lut + 16, a);
//        if (*p != a){std::cerr<<"not a hex digit";}

        char b = toupper(sKey[i + 1]);
        const char* q = std::lower_bound(lut, lut + 16, b);
//        if (*q != b){std::cerr<<"not a hex digit";}

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    sKey=output;
    //end magic

    size_t iOutLen;
    char *out=base64_encode((const unsigned char*)sKey.c_str(),sKey.length(),&iOutLen);

    std::string sFinal(out);
    sFinal.resize(iOutLen);

    SendHTTPHandshakeResponse(sFinal);
    m_bShutdown=false;
    m_bReady=true;
}

WSConnection::~WSConnection(){

}

}
