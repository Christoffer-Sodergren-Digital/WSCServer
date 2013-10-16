#include <iostream>
#include <vector>
#include <boost\algorithm\string.hpp>
#include <sstream>
#include <mysql_connection.h>
#include <mysql_driver.h>
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
	m_iLastUpdate=clock();;
}

Player::Player(){
	m_pxInstance=NULL;
	m_pxConnection=NULL;
	m_bLoggedIn=false;
	m_bShutdown=false;
	m_sName="NoName";
	m_iPlayerID=0;
	m_iLastUpdate=clock();;
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
	if(m_bLoggedIn){
		clock_t iT=clock();
		clock_t diff=iT-m_iLastUpdate;
		if(diff>=((float)16/CLOCKS_PER_SEC)){
			Snapshot();
			m_iLastUpdate=iT;
		}
		
	}
}

void Player::UpdateDB(){
	MySQLDB *pxDB=MySQLDB::Instance();
	pxDB->Reconnect();
	sql::Connection *pxCon=pxDB->GetConnection();

	

	/*try{
		m_pxUpdateStatement->setDouble(1,m_vPos.m_fX);
		m_pxUpdateStatement->setDouble(2,m_vPos.m_fX);
		m_pxUpdateStatement->setInt(3,m_iPlayerID);
		int res=m_pxUpdateStatement->executeUpdate();
	}catch(SQLException & e){
		std::cerr<<e.what()<<std::endl;
		return;
	}*/
}

void Player::HandleFrame(unsigned char *p_pucData){
	int iLen=(int)strlen((const char*)p_pucData);
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
	}else if(ucOpCode==WS::APP_INPUTBUFFER){
		ParseInputBuffer(p_pucData);
	}
}
void Player::ParseInputBuffer(unsigned char *p_pucData){
	if(p_pucData==NULL){return;}
	size_t iLen=strlen((char*)p_pucData);
	int i,iC=(int)iLen;
	for(i=1;i<iC;i++){
		unsigned char ucK=p_pucData[i]-48;
		if(ucK==DOWN){
			m_vPos.m_fY+=2;
		}else if(ucK==LEFT){
			m_vPos.m_fX-=2;
		}else if(ucK==UP){
			m_vPos.m_fY-=2;
		}else if(ucK==RIGHT){
			m_vPos.m_fX+=2;
		}
	}
}
void Player::Snapshot(){
	int iPosX=(int)m_vPos.m_fX;
	int iPosY=(int)m_vPos.m_fY;

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
	
}

void Player::AttemptLogin(unsigned char *p_pucData){
	MySQLDB *pxDB=MySQLDB::Instance();
	pxDB->Reconnect();
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
		pxPS=pxCon->prepareStatement("SELECT u.id, c.name FROM ca_user AS u LEFT JOIN ca_character AS c ON c.id = u.id WHERE u.email=? AND u.password=? LIMIT 0 , 30");
		pxPS->setString(1,sName);
		pxPS->setString(2,sHash);
		if(!pxPS->execute()){m_bShutdown=true;return;}
	}
	catch (SQLException &e){
		std::cerr<<e.what()<<std::endl;
	}

	sql::ResultSet *pxRS=pxPS->getResultSet();
	int iCount=(int)pxRS->rowsCount();

	if(iCount<=0){m_bShutdown=true;return;}
	if(iCount>1){std::cerr<<"DATABASE: More than one matching user account for login\n";m_bShutdown=true;return;}

	if(pxRS->next()){
		m_sName=pxRS->getString("name");
	}

	m_bLoggedIn=true;

	if(pxRS->next()){
		m_iPlayerID=pxRS->getInt("id");
	}
	
	delete pxPS;
	delete pxRS;
	
	pxDB->Reconnect();
	pxCon=pxDB->GetConnection();

	sql::Statement *pxST=NULL;
	try{
		pxST=pxCon->createStatement();
		pxST->execute("UPDATE ca_character SET zone=0 WHERE id=1");
	}catch (SQLException & e){
		std::cerr<<e.what()<<std::endl;
	}

	delete pxST;

	WS::WSApplicationDataFrame *pxAppFrame=new WS::WSApplicationDataFrame((unsigned char*)"ok",WS::APP_LOGIN);
	WS::WSFrame *pxWSFrame=new WS::WSFrame(pxAppFrame->Frame(),WS::OP_BINARY);
	m_pxConnection->Send(pxWSFrame->Data(),pxWSFrame->FrameSize());

	delete pxAppFrame;
	delete pxWSFrame;
}

void Player::PreDisconnect(){
	MySQLDB *pxDB=MySQLDB::Instance();
	pxDB->Reconnect();
	sql::Connection *pxCon=pxDB->GetConnection();
	sql::PreparedStatement *pxPS=NULL;

	try{
		pxPS=pxCon->prepareStatement("SELECT SQL_NO_CACHE guest FROM ca_user WHERE id=?");
		pxPS->setInt(1,m_iPlayerID);
		if(!pxPS->execute()){m_bShutdown=true;return;}
	}
	catch (SQLException &e){
		std::cerr<<e.what()<<std::endl;
	}

	sql::ResultSet *pxRS=pxPS->getResultSet();
	bool bShouldDelete=false;
	if(pxRS->next()){
		int iGuest=pxRS->getInt("guest");
		if(iGuest>0){
			bShouldDelete=true;
		}
	}

	delete pxPS;
	delete pxRS;
	
	if(bShouldDelete){
		pxDB->Reconnect();
		pxCon=pxDB->GetConnection();
		try{
			pxPS=pxCon->prepareStatement("DELETE FROM ca_user WHHERE id=?");
			pxPS->setInt(1,m_iPlayerID);
			if(!pxPS->execute()){m_bShutdown=true;std::cerr<<"Could not delete guest account with id: "<<m_iPlayerID<<std::endl;return;}
		}catch (SQLException &e){
			std::cerr<<e.what()<<std::endl;
		}

		delete pxPS;
	}
}


std::stringstream & operator<<(std::stringstream & p_xSS, Player & p_xP){
	p_xSS<<p_xP.m_iPlayerID<<p_xP.m_vPos.m_fX<<p_xP.m_vPos.m_fY;
	return p_xSS;
}