#pragma once
#ifndef CAGER_CHAT_H
#define CAGER_CHAT_H

#include <list>
#include <string>
#include "Player.h"
#include "WSConnection.h"
#include "ChatFrame.h"

using namespace WS;

class CagerChat;

//class IChatCommand{
//public:
//	virtual ~IChatCommand(){};
//
//	virtual void DoAction()=0;
//	virtual char *Command() = 0;
//};
//
//class ChatCommand : public IChatCommand{
//public:
//	ChatCommand(CagerChat *p_pxChat,WSConnection *p_pxCon, std::string & p_sData):m_sMsg(p_sData), m_pxChat(p_pxChat), m_pxCon(p_pxCon){}
//
//	virtual void DoAction(){}
//	virtual char *Command(){return m_sCommand;}
//
//protected:
//	char *m_sCommand;
//	std::string m_sMsg;
//	CagerChat *m_pxChat;
//	WSConnection *m_pxCon;
//};
//
//class SayCommand : public ChatCommand{
//public:
//	SayCommand(CagerChat *p_pxChat,WSConnection *p_pxConnection, std::string & p_sData) : ChatCommand(p_pxChat,p_pxConnection,p_sData){
//		m_sCommand=new char[3];
//		strcpy(m_sCommand,"say");
//		m_sCommand[3]='\0';
//	}
//
//	virtual void DoAction();
//};
//
//class ListCommand : public ChatCommand{
//public:
//	ListCommand(CagerChat *p_pxChat,WSConnection *p_pxConnection, std::string & p_sData) : ChatCommand(p_pxChat,p_pxConnection,p_sData){
//		m_sCommand=new char[4];
//		strcpy(m_sCommand,"list");
//		m_sCommand[4]='\0';
//	}
//
//	virtual void DoAction();
//};

class CagerChat{

public:
	CagerChat();
	~CagerChat();

	void Join(Player *p_pxCon);
	void Leave(Player *p_pxCon);

	void Broadcast(std::string & p_sMsg,Player *p_pxOrigin,bool p_bEcho=true);

	void ParseChatFrame(ChatFrame *p_pxFrame,Player *p_pxOrigin);

	const std::list<WSConnection*> & Participants(){return m_xParticipants;}
private:

	std::list<WSConnection*> m_xParticipants;
};


#endif