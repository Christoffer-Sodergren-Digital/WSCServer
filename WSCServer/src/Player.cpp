#include <iostream>
#include <vector>
#include <boost\algorithm\string.hpp>
#include <prepared_statement.h>
#include "Player.h"
#include "hl_md5wrapper.h"
#include "FrameManager.h"
#include "CagerChat.h"
#include "MySQLDB.h"


using namespace WS;

Player::Player(WSConnection *p_pxConnection,CagerInstance *p_pxInstance){
	m_pxConnection=p_pxConnection;
	m_pxInstance=p_pxInstance;
	m_bShutdown=false;
	m_bLoggedIn=false;
}

Player::Player(){
	m_pxInstance=NULL;
	m_pxConnection=NULL;
	m_bLoggedIn=false;
	m_bShutdown=false;
}

Player::~Player(){
	if(m_pxConnection->IsConnected()){
		m_pxConnection->Close();
	}
	delete m_pxConnection;
}

void Player::Update(){
	WSConnection *pxCon=m_pxConnection;
	int iPending=pxCon->PendingData();
    if(iPending>0){
        if(pxCon->CanRecvData()){
			WSFrame *pxFrame=pxCon->ReceiveFrame();
			//WSApplicationDataFrame *pxAppFrame=new WSApplicationDataFrame(pxFrame->ApplicationData(),pxFrame->OpCode());
			if(pxFrame){
				HandleFrame(pxFrame->ApplicationData());
				delete pxFrame;
			}
		}
    }
    if(pxCon->HasHalfClose()){
        std::cout<<"Connection closed: ";
        pxCon->GetRemoteAddr().Dump();
        pxCon->Close();
		m_bShutdown=true;
    }
}

void Player::HandleFrame(unsigned char *p_pucData){
	int iLen=strlen((const char*)p_pucData);
	if(iLen<2){return;}
	unsigned char ucOpCode=p_pucData[0];

	if(!m_bLoggedIn&&ucOpCode!=WS::APP_LOGIN){
		m_bShutdown=true;
		return;
	}

	//WSApplicationDataFrame *pxFrame=ApplicationFrameManager::ConstructFrame(ucOpCode);
	if(ucOpCode==WS::APP_MSG){
		ChatFrame *pxFrame=new ChatFrame();
		pxFrame->MakeFrame(p_pucData,WS::APP_MSG);
		m_pxInstance->Chat()->ParseChatFrame(pxFrame);
	}else if(ucOpCode==WS::APP_LOGIN){
		AttemptLogin(p_pucData);
	}
}

void Player::AttemptLogin(unsigned char *p_pucData){
	MySQLDB *pxDB=MySQLDB::Instance();
	sql::Connection *pxCon=pxDB->GetConnection();
	
	std::string sData=(char*)p_pucData;
	std::vector<std::string> xTokens;
	boost::split(xTokens, sData.substr(1), boost::is_any_of(";"));
	if(xTokens.size()<2){m_bShutdown=true;return;}

	md5wrapper xMD5;
	std::string sHash=xMD5.getHashFromString(xTokens[1]);
	std::string sName=xTokens[0];

	sql::PreparedStatement *pxPS=pxCon->prepareStatement("SELECT id FROM ca_user WHERE name=? AND password=?");
	pxPS->setString(1,sName);
	pxPS->setString(2,sHash);
	if(!pxPS->execute()){m_bShutdown=true;return;}

	sql::ResultSet *pxRS=pxPS->getResultSet();
	int iCount=pxRS->rowsCount();

	if(iCount<=0){m_bShutdown=true;return;}
	if(iCount>1){std::cerr<<"DATABASE: More than one matching user account for login\n";m_bShutdown=true;return;}
	m_bLoggedIn=true;

	delete pxPS;

	WS::WSApplicationDataFrame *pxAppFrame=new WS::WSApplicationDataFrame((unsigned char*)"ok",WS::APP_LOGIN);
	WS::WSFrame *pxWSFrame=new WS::WSFrame(pxAppFrame->Frame(),WS::OP_BINARY);
	m_pxConnection->Send(pxWSFrame->Data(),pxWSFrame->FrameSize());
}