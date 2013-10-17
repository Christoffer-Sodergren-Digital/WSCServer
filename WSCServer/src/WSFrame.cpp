#include "WSFrame.h"
#include <cstring>
#include <cmath>

namespace WS{

WSFrame::WSFrame(){
    m_bValid=false;
    m_ucFin=0;
    m_ucMask=0;
    m_ucOpcode=0;
    m_uiPayloadLen=0;
    m_pcApplicationData=0;
    m_puiMask=0;
    m_uiFrameSize=0;
}


//Only for parsing
WSFrame::WSFrame(char *p_pcData)
{
    m_bValid=false;
    m_ucFin=0;
    m_ucMask=0;
    m_ucOpcode=0;
    m_uiPayloadLen=0;
    m_pcApplicationData=0;
    m_puiMask=0;
    m_uiFrameSize=0;
    ParseFrame(p_pcData);

}
WSFrame::WSFrame(const unsigned char *p_pcData,int p_iSize, WsOpcode p_eOpCode){
	m_bValid=false;
    m_ucFin=0;
    m_ucMask=0;
    m_ucOpcode=0;
    m_uiPayloadLen=0;
    m_pcApplicationData=0;
    m_puiMask=0;
    m_uiFrameSize=0;
	MakeFrame(p_pcData,p_iSize,p_eOpCode);
}

WSFrame::~WSFrame(){
    if(m_pcApplicationData){
        delete [] m_pcApplicationData;
    }
    if(m_puiMask){
        delete [] m_puiMask;
    }
}

void WSFrame::ParseFrame(char *p_pcData){
    int iInLen=strlen(p_pcData);
    if(iInLen<3){return;}

    unsigned char ucOpCode=(p_pcData[0] & 0x0F);
    unsigned char ucFin=(p_pcData[0] >> 7) & 0x01;
    unsigned char ucMasked=(p_pcData[1]>>7) & 0x01;

    unsigned int iPayloadLen=0;
    int i=2;
    int iLenField=0; //manual bit manip, oh well
    for(int j=6,n=0;j>=0;j--,n++){
        bool bSet=(p_pcData[1] & (1<<j));
        iLenField+=(bSet)?(pow(2,j)):0;
    }

    unsigned int uiMask=0;

    if(iLenField<=125){
        iPayloadLen=iLenField;
    }else if(iLenField==126){
        iPayloadLen=p_pcData[3]+(p_pcData[2]<<8);
        i+=2;
    }else if(iLenField==127){
        for(int j=2;j<10;j++){
            iPayloadLen+=(p_pcData[i]*pow(2,8*j));
        }
        i+=8;
    }

    if(iInLen<iPayloadLen+i){
       return;
    }
    if(ucMasked){
        uiMask=*((unsigned int*)(p_pcData+i));
        i+=MASK_SIZE;
        unsigned char *ucBuff=(unsigned char*)(p_pcData+i);
        int iIdx,iC=iPayloadLen;
        for(iIdx=0;iIdx<iC;iIdx++){
            ucBuff[iIdx]=ucBuff[iIdx]^((unsigned char*)(&uiMask))[iIdx%4];
        }
    }
    m_pcApplicationData=new unsigned char[iPayloadLen+1];
    memcpy(m_pcApplicationData, p_pcData+i, iPayloadLen);
    m_pcApplicationData[iPayloadLen]='\0';

    m_ucFin=ucFin;
    m_ucOpcode=ucOpCode;
    m_ucMask=ucMasked;
    m_uiPayloadLen=iPayloadLen;
    m_bValid=true;
    m_uiFrameSize=iInLen;

    m_pcData=new unsigned char[iInLen];
    memcpy(m_pcData,p_pcData,iInLen);
}

void WSFrame::MakeFrame(const unsigned char *p_pucApplicationData, int p_iSize, WsOpcode p_eOpcode){
    int iPos=0;
    int iSize=p_iSize;//.length();
    int iBuffSize=0;
    int iOffset=0;
    unsigned char *pBuffer;

    if(iSize<=125){
        pBuffer=new unsigned char[iSize+3];
        unsigned char ucWsFrame=0x08; //TODO: i'm assuming single frame data, don't.
        ucWsFrame=((ucWsFrame<<4) | (p_eOpcode & 0x0f));
        pBuffer[iOffset++]=ucWsFrame;
        ucWsFrame=0;
        ucWsFrame=((ucWsFrame<<1) | iSize);
        pBuffer[iOffset++]=ucWsFrame;
    }else{ //(iSize<=65535){
        pBuffer=new unsigned char[iSize+5];
        unsigned char ucWsFrame=0x08; //TODO: i'm assuming single frame data, don't.
        ucWsFrame=((ucWsFrame<<4) | (p_eOpcode & 0x0f));
        pBuffer[iOffset++]=ucWsFrame;
        ucWsFrame=0;
        ucWsFrame=((ucWsFrame<<1) | 126);
        pBuffer[iOffset++]=ucWsFrame;
        pBuffer[iOffset++]=(iSize>>8) & 0xFF;
        pBuffer[iOffset++]=iSize & 0xFF;
    }//else{
    //    //todo add 64bit
    //}

    memcpy((void*)(pBuffer+iOffset),p_pucApplicationData,iSize);
    m_pcApplicationData=new unsigned char[iSize+1];
    memcpy(m_pcApplicationData,p_pucApplicationData,iSize);
    m_pcApplicationData[iSize]='\0';
    m_uiPayloadLen=iSize;
    m_uiFrameSize=iSize+iOffset;
	m_ucOpcode=p_eOpcode;
    m_pcData=pBuffer;
	m_pcData[iOffset+iSize]=0;
}

unsigned char *WSFrame::Data(){
    return m_pcData;
}

}
