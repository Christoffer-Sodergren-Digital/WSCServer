#include "ChatFrame.h"

#include <cstring>
#include <string>

ChatFrame::ChatFrame(){
	m_bWhisper=false;
}

ChatFrame::~ChatFrame(){
    //dtor
}

void ChatFrame::ParseFrame(unsigned char *p_pcData){
    CleanFrame();
    int iLen=strlen((const char*)p_pcData);
    if(iLen<2){return;}

    m_ucOpCode=p_pcData[0];
    m_pucData=new unsigned char[iLen];
    memcpy(m_pucData,p_pcData,iLen);
    m_uiDataSize=iLen;

	unsigned char ucControlFlag=p_pcData[1];
	m_bWhisper=(ucControlFlag & 0x01);
	int iTargetLen=0;
	std::string sMsg=(const char*)(p_pcData+1);
	if(m_bWhisper){
		iTargetLen=((ucControlFlag>>1) & 0xf);
		m_sWhisperTarget=sMsg.substr(0,iTargetLen);
		sMsg=sMsg.substr(iTargetLen);
	}
	m_sMessage=sMsg;

	Finalize();
}

void ChatFrame::MakeFrame(unsigned char *p_pucData,ApplicationFrameOpCode p_eOpCode){
	 CleanFrame();
	unsigned char ucControlFlag=(m_bWhisper & 0x1);
	
    int iLen=strlen((const char *)p_pucData);
    if(iLen<2){return;}
   
	m_ucOpCode=p_eOpCode;

	int iDataSize=iLen+1;
	m_pucData=new unsigned char[iDataSize];
    strcpy((char*)m_pucData,(const char*)p_pucData);
    m_uiDataSize=iDataSize;
	

	std::string sMsg=(const char*)(m_pucData);
	/*int iTargetLen=0;
	if(m_bWhisper){
		iTargetLen=((ucControlFlag>>1) & 0xf);
		m_sWhisperTarget=sMsg.substr(0,iTargetLen);
		sMsg=sMsg.substr(iTargetLen);
	}*/

	m_sMessage=sMsg;
	

	Finalize();
}
