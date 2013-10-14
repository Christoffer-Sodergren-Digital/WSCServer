#include "ApplicationDataFrame.h"
#include <cstring>

namespace WS{


WSApplicationDataFrame::WSApplicationDataFrame(){
   CleanFrame();
}

WSApplicationDataFrame::WSApplicationDataFrame(unsigned char* p_pcData, ApplicationFrameOpCode p_eOpCode){
    MakeFrame(p_pcData,p_eOpCode);
}

WSApplicationDataFrame::~WSApplicationDataFrame(){
    CleanFrame();
}

void WSApplicationDataFrame::CleanFrame(){
    if(m_pucData){
        delete m_pucData;
    }
    if(m_pucFrame){
        delete m_pucFrame;
    }
    m_ucOpCode=0;
    m_uiDataOffset=1;
    m_uiFrameSize=0;
    m_uiDataSize=0;
    m_bValidFrame=false;
    m_pucData=0;
    m_pucFrame=0;
}

void WSApplicationDataFrame::MakeFrame(unsigned char* p_pcData, ApplicationFrameOpCode p_eOpCode){
    CleanFrame();
    int iLen=strlen((const char *)p_pcData);
   /* if(iLen<2){return;}*/
    m_ucOpCode=p_eOpCode;

    m_uiDataSize=iLen;
    m_pucData=new unsigned char[iLen+1];
    memcpy(m_pucData,p_pcData,iLen);
    m_uiDataSize=iLen;
	m_pucData[iLen]=0;
}

void WSApplicationDataFrame::Finalize(){
    m_uiFrameSize=m_uiDataSize+m_uiDataOffset;
    m_pucFrame=new unsigned char[m_uiDataSize+m_uiDataOffset+1];
    m_pucFrame[0]=m_ucOpCode;
    memcpy(m_pucFrame+m_uiDataOffset,m_pucData,m_uiDataSize);
	m_pucFrame[m_uiDataSize+m_uiDataOffset]=0;

    m_bValidFrame=true;
}

void WSApplicationDataFrame::ParseFrame(unsigned char* p_pcData){
    CleanFrame();
    int iLen=strlen((const char*)p_pcData);
    if(iLen<2){return;}

    m_ucOpCode=p_pcData[0];
    m_pucData=new unsigned char[iLen+1];
    memcpy(m_pucData,p_pcData,iLen);
	m_pucData[iLen]=0;
    m_uiDataSize=iLen;

    Finalize();
}

unsigned char *WSApplicationDataFrame::Frame(){
    if(!m_bValidFrame){
		Finalize();
	}
    return m_pucFrame;
}

};
