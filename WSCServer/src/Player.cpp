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
#include "EntityID.h"


using namespace WS;

Player::Player(WSConnection *p_pxConnection,CagerInstance *p_pxInstance){
	m_pxConnection=p_pxConnection;
	m_pxInstance=p_pxInstance;
	m_bLoggedIn=false;
	m_sName="NoName";
	m_iPlayerID=0;
	m_iLastUpdate=clock();
	m_bNeedFullSnapshot=true;
	m_ucEntityType=ENT_PLAYER;
}

Player::Player(){
	m_pxInstance=NULL;
	m_pxConnection=NULL;
	m_bLoggedIn=false;
	m_sName="NoName";
	m_iPlayerID=0;
	m_iLastUpdate=clock();
	m_bNeedFullSnapshot=true;
	m_ucEntityType=ENT_PLAYER;
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
				if(pxFrame->OpCode()==WS::OP_CLOSE){
					m_pxConnection->SetShutdown();
				}else{
					HandleFrame(pxFrame->ApplicationData(),pxFrame->PayloadLen());
				}
				delete pxFrame;
			}
		}
    }
    if(pxCon->HasHalfClose()){
        std::cout<<"Connection closed: ";
        pxCon->GetRemoteAddr().Dump();
		m_pxConnection->SetShutdown();
    }
}

void Player::HandleFrame(unsigned char *p_pucData,int p_iSize){
	int iLen=p_iSize;
	if(iLen<2){return;}
	unsigned char ucOpCode=p_pucData[0];

	if(!m_bLoggedIn&&ucOpCode!=WS::APP_LOGIN){
		m_pxConnection->SetShutdown();
		return;
	}

	//TODO: Fix this to create seperate message handlers instead
	if(ucOpCode==WS::APP_MSG){
		ChatFrame *pxFrame=new ChatFrame();
		pxFrame->MakeFrame(p_pucData,iLen,WS::APP_MSG);
		m_pxInstance->Chat()->ParseChatFrame(pxFrame,this);
		delete pxFrame;
	}else if(ucOpCode==WS::APP_LOGIN){
		AttemptLogin(p_pucData);
	}else if(ucOpCode==WS::APP_INPUTBUFFER){
		ParseInputBuffer(p_pucData,iLen);
	}
}
void Player::ParseInputBuffer(unsigned char *p_pucData,int p_iLen){
	if(p_pucData==NULL){return;}
	size_t iLen=p_iLen;
	bool bC=true;
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
		}else{
			bC=false;
		}
	}
	m_bNetChanged=bC;
}

void Player::AttemptLogin(unsigned char *p_pucData){
	if(!m_pxConnection){return;}
	MySQLDB *pxDB=MySQLDB::Instance();
	pxDB->Reconnect();
	sql::Connection *pxCon=pxDB->GetConnection();

	std::string sData=(char*)p_pucData;
	std::vector<std::string> xTokens;
	boost::split(xTokens, sData.substr(1), boost::is_any_of(";"));
	if(xTokens.size()<2){m_pxConnection->SetShutdown();return;}

	md5wrapper xMD5;
	std::string sHash=xMD5.getHashFromString(xTokens[1]);
	std::string sName=xTokens[0];
	sql::PreparedStatement *pxPS=NULL;
	try{
		pxPS=pxCon->prepareStatement("SELECT u.id, c.name FROM ca_user AS u LEFT JOIN ca_character AS c ON c.id = u.id WHERE u.email=? AND u.password=? LIMIT 0 , 30");
		pxPS->setString(1,sName);
		pxPS->setString(2,sHash);
		if(!pxPS->execute()){m_pxConnection->SetShutdown();return;}
	}
	catch (SQLException &e){
		std::cerr<<e.what()<<std::endl;
	}

	sql::ResultSet *pxRS=pxPS->getResultSet();
	int iCount=(int)pxRS->rowsCount();

	if(iCount<=0){m_pxConnection->SetShutdown();return;}
	if(iCount>1){
		std::cerr<<"DATABASE: More than one matching user account for login\n";
		m_pxConnection->SetShutdown();
		return;
	}

	if(pxRS->next()){
		m_sName=pxRS->getString("name");
		m_iPlayerID=pxRS->getInt("id");
	}

	m_bLoggedIn=true;
	
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

	WS::WSApplicationDataFrame *pxAppFrame=new WS::WSApplicationDataFrame((unsigned char*)"ok",2,WS::APP_LOGIN);
	//pxAppFrame->Finalize();
	WS::WSFrame *pxWSFrame=new WS::WSFrame(pxAppFrame->Frame(),pxAppFrame->FrameSize(),WS::OP_BINARY);
	m_pxConnection->Send(pxWSFrame->Data(),pxWSFrame->FrameSize());

	delete pxAppFrame;
	delete pxWSFrame;
}

void Player::PreDisconnect(){
	if(!m_pxConnection){return;}

	std::cout<<"Connection closed: ";
	m_pxConnection->GetRemoteAddr().Dump();
	m_pxConnection->Close();

	MySQLDB *pxDB=MySQLDB::Instance();
	pxDB->Reconnect();
	sql::Connection *pxCon=pxDB->GetConnection();
	sql::PreparedStatement *pxPS=NULL;

	try{
		pxPS=pxCon->prepareStatement("SELECT SQL_NO_CACHE guest FROM ca_user WHERE id=?");
		pxPS->setInt(1,m_iPlayerID);
		if(!pxPS->execute()){m_pxConnection->SetShutdown();return;}
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
			if(!pxPS->execute()){
				m_pxConnection->SetShutdown();
				std::cerr<<"Could not delete guest account with id: "<<m_iPlayerID<<std::endl;
				return;
			}
		}catch (SQLException &e){
			std::cerr<<e.what()<<std::endl;
		}

		delete pxPS;
	}
}


std::ostream & operator<<(std::ostream & p_xSS, Player & p_xP){
	p_xSS<<p_xP.m_ucEntityType;
	BaseEntity & xBase(p_xP);
	p_xSS<<xBase;
	p_xSS<<p_xP.m_vPos;
	return p_xSS;
}