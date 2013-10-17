#include "CagerChat.h"
#include "ChatFrame.h"
#include <iostream>

//void SayCommand::DoAction(){
//	if(!m_pxChat){return;}
//	m_pxChat->Broadcast(m_sMsg,m_pxCon);
//}
//
//void ListCommand::DoAction(){
//	ChatFrame *pxFrame=new ChatFrame();
//	pxFrame->Whisper(false);
//	pxFrame->MakeFrame((unsigned char*)m_sMsg.c_str(),WS::APP_MSG);
//	
//	WSFrame *pxOut=new WSFrame();
//	pxOut->MakeFrame(pxFrame->Frame(),OP_TEXT);
//	std::string sMsg=(const char*)pxOut->Data();
//	m_pxChat->Broadcast(sMsg,m_pxCon);
//}

CagerChat::CagerChat(){
}

CagerChat::~CagerChat(){
}

void CagerChat::ParseChatFrame(ChatFrame *p_pxFrame,Player *p_pxOrigin){
	std::string sMsg=p_pxFrame->Message();
	Broadcast(sMsg,p_pxOrigin);
}

void CagerChat::Leave(Player *p_pxCon){
	std::list<WSConnection *>::iterator it=m_xParticipants.begin();
	for(;it!=m_xParticipants.end();++it){
		if((*it)==p_pxCon->Connection()){
			it=m_xParticipants.erase(it);
			std::string sMsg;
			sMsg=p_pxCon->Name()+" just joined the chat.";
		//	Broadcast(sMsg, p_pxCon);
			break;
		}
	}
}

void CagerChat::Join(Player *p_pxCon){
	std::list<WSConnection *>::iterator it=m_xParticipants.begin();
	bool bFound=false;
	for(;it!=m_xParticipants.end();++it){
		if((*it)==p_pxCon->Connection()){
			bFound=true;
			break;
		}
	}
	if(!bFound){
		m_xParticipants.push_back(p_pxCon->Connection());
	}
	std::string sMsg;
	sMsg=p_pxCon->Name()+" just joined the chat.";
//	Broadcast(sMsg, p_pxCon);
}

void CagerChat::Broadcast(std::string & p_sMsg, Player *p_pxOrigin, bool p_bEcho){
	std::string sMsg=p_pxOrigin->Name()+": ";
	sMsg+=p_sMsg;
	ChatFrame *pxFrame=new ChatFrame();
	pxFrame->MakeFrame((unsigned char*)sMsg.c_str(),sMsg.length(),APP_MSG);
	WSFrame *pxOut=new WSFrame();
	pxOut->MakeFrame(pxFrame->Frame(),pxFrame->FrameSize(),WS::OP_BINARY);
	
	std::list<WSConnection*>::iterator it;
	for(it=m_xParticipants.begin();it!=m_xParticipants.end();++it){
		WSConnection *pxCon=(*it);
		if((*it)==p_pxOrigin->Connection()&&!p_bEcho){continue;}
		
		int iSent=pxCon->Send(pxOut->Data(),pxOut->FrameSize());
		//std::cout<<"Sent "<<p_sMsg<<"\n";
	}
	delete pxFrame;
	delete pxOut;
}