#include <iostream>
#include <vector>
#include <boost\algorithm\string.hpp>
#include <prepared_statement.h>
#include <sstream>
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
	m_sName="NoName";
	m_iPlayerID=0;
}

Player::Player(){
	m_pxInstance=NULL;
	m_pxConnection=NULL;
	m_bLoggedIn=false;
	m_bShutdown=false;
	m_sName="NoName";
	m_iPlayerID=0;
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

	//TODO: Fix this to create seperate message handlers instead
	if(ucOpCode==WS::APP_MSG){
		ChatFrame *pxFrame=new ChatFrame();
		pxFrame->MakeFrame(p_pucData,WS::APP_MSG);
		m_pxInstance->Chat()->ParseChatFrame(pxFrame,this);
	}else if(ucOpCode==WS::APP_LOGIN){
		AttemptLogin(p_pucData);
	}else if(ucOpCode==WS::APP_SNAPSHOT){
		Snapshot();
	}
}

void Player::Snapshot(){
	MySQLDB *pxDB=MySQLDB::Instance();
	pxDB->Reconnect();
	sql::Connection *pxCon=pxDB->GetConnection();
	sql::PreparedStatement *pxPS=NULL;
	sql::ResultSet *pxRS=NULL;
	
	try{
		pxPS=pxCon->prepareStatement("SELECT SQL_NO_CACHE posx,posy FROM ca_character WHERE id=?");
		pxPS->setInt(1,m_iPlayerID);
		pxRS=pxPS->executeQuery();
	}catch (SQLException &e){
			std::cerr<<e.what()<<std::endl;
			m_bShutdown=true;
			return;
	}

	int iPosX=0;
	int iPosY=0;

	if(pxRS->next()){
		iPosX=pxRS->getInt("posx");
		iPosY=pxRS->getInt("posy");
	}



	unsigned char *sBuff=new unsigned char[5];

	sBuff[0]=(iPosX>>4);
	sBuff[1]=(iPosX&0xff);
	sBuff[2]=(iPosY>>4);
	sBuff[3]=(iPosY&0xff);
	
	sBuff[4]=0;

	WS::WSApplicationDataFrame *pxAppFrame=new WS::WSApplicationDataFrame(sBuff,WS::APP_SNAPSHOT);
	WS::WSFrame *pxWSFrame=new WS::WSFrame(pxAppFrame->Frame(),WS::OP_BINARY);
	m_pxConnection->Send(pxWSFrame->Data(),pxWSFrame->FrameSize());
	

	delete pxAppFrame;
	delete pxWSFrame;
	delete sBuff;
	delete pxPS;
	delete pxRS;
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
	sql::PreparedStatement *pxPS=NULL;
	try{
		pxPS=pxCon->prepareStatement("SELECT SQL_NO_CACHE id FROM ca_user WHERE name=? AND password=?");
		pxPS->setString(1,sName);
		pxPS->setString(2,sHash);
		if(!pxPS->execute()){m_bShutdown=true;return;}
	}
	catch (SQLException &e){
		std::cerr<<e.what()<<std::endl;
	}

	sql::ResultSet *pxRS=pxPS->getResultSet();
	int iCount=pxRS->rowsCount();

	if(iCount<=0){m_bShutdown=true;return;}
	if(iCount>1){std::cerr<<"DATABASE: More than one matching user account for login\n";m_bShutdown=true;return;}
	m_bLoggedIn=true;

	if(pxRS->next()){
		m_iPlayerID=pxRS->getInt("id");
	}
	
	m_sName=sName;
	delete pxPS;
	delete pxRS;

	WS::WSApplicationDataFrame *pxAppFrame=new WS::WSApplicationDataFrame((unsigned char*)"ok",WS::APP_LOGIN);
	WS::WSFrame *pxWSFrame=new WS::WSFrame(pxAppFrame->Frame(),WS::OP_BINARY);
	m_pxConnection->Send(pxWSFrame->Data(),pxWSFrame->FrameSize());

	delete pxAppFrame;
	delete pxWSFrame;

	/*try{
		pxPS=pxCon->prepareStatement("SELECT name,level,hp,mana,str,dex,inte,xp,posx,posy,zone FROM ca_character WHERE id=?");
		pxPS->setInt(1,m_iPlayerID);
		if(!pxPS->execute()){m_bShutdown=true;return;}
	}catch(SQLException &e){
			std::cerr<<e.what()<<std::endl;
			m_bShutdown=true;
			return;
	}

	pxRS=pxPS->getResultSet();
	if(pxRS->rowsCount()<=0){m_bShutdown=true;return;}

	int iPosX=0;
	int iPosY=0;

	if(pxRS->next()){
		iPosX=pxRS->getInt("posx");
		iPosY=pxRS->getInt("posy");
	}


	int iPacked=iPosX;
	iPacked=(iPacked<<8)|(iPosY&0xffff);

	unsigned char *sBuff=new unsigned char[5];
	memcpy(sBuff,&iPosX,2);
	memcpy(sBuff+2,&iPosY,2);
	

	pxAppFrame=new WS::WSApplicationDataFrame(sBuff,WS::APP_SNAPSHOT);
	pxWSFrame=new WS::WSFrame(pxAppFrame->Frame(),WS::OP_BINARY);
	m_pxConnection->Send(pxWSFrame->Data(),pxWSFrame->FrameSize());
	

	delete pxAppFrame;
	delete pxWSFrame;
	delete sBuff;
	delete pxPS;
	delete pxRS;*/

}