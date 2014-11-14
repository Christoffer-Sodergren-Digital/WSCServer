#include <iostream>
#include <vector>
#include <boost\algorithm\string.hpp>
#include <sstream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include "Item.h"
#include "Player.h"
#include "hl_md5wrapper.h"
#include "FrameManager.h"
#include "CagerChat.h"
#include "MySQLDB.h"
#include "EntityID.h"
<<<<<<< HEAD
#include "Zone.h"
#include "Item.h"
=======

>>>>>>> parent of fa8c75e... Entities, collisions and movement

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
	m_pxPath=NULL;
	m_fLerpT=0.0f;
	m_fLerpSpeed=60.0f;
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
	m_pxPath=NULL;
	m_fLerpT=0.0f;
	m_fLerpSpeed=60.0f;
}

Player::~Player(){
	if(m_pxConnection->IsConnected()){
		m_pxConnection->Close();
	}
	delete m_pxConnection;
}

void Player::Update(clock_t p_iDiff){
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
<<<<<<< HEAD
=======
        std::cout<<"Connection closed: ";
        pxCon->GetRemoteAddr().Dump();
>>>>>>> parent of fa8c75e... Entities, collisions and movement
		m_pxConnection->SetShutdown();
    }
	if(m_pxPath!=NULL){
		InterpolatePosition(p_iDiff);
	}
}

void Player::InterpolatePosition(clock_t p_iDiff){
	if(m_pxPath->size()<=0){
		delete m_pxPath;
		m_pxPath=NULL;
		return;
	}

	Vec2 vTarget=m_pxPath->back();

	float fT=m_fLerpT;
	m_fLerpT+=p_iDiff/m_fLerpSpeed;

	if(m_fLerpT>=1.0f){
		m_fLerpT=0.0f;
		m_xAABB.m_vOrg=vTarget;
		m_pxPath->pop_back();
		return;
	}

	float fX0=m_xAABB.m_vOrg.m_fX;
	float fX1=vTarget.m_fX;
	m_xAABB.m_vOrg.m_fX=fX0+(fX1-fX0)*fT;
	
	float fY0=m_xAABB.m_vOrg.m_fY;
	float fY1=vTarget.m_fY;
	m_xAABB.m_vOrg.m_fY=fY0+(fY1-fY0)*fT;
	
	m_bNetChanged=true;
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
	}else if(ucOpCode==WS::APP_PLAYER_FETCH_INVENTORY){
		FetchInventory();
	}else if(ucOpCode==WS::APP_PLAYER_EQUIP_ITEM){
		EquipItem(p_pucData,iLen);
	}
}
void Player::EquipItem(unsigned char *p_pucData,int p_iLen){
	if(!m_pxConnection){return;}
	MySQLDB *pxDB=MySQLDB::Instance();
	pxDB->Reconnect();
	sql::Connection *pxCon=pxDB->GetConnection();

	if(p_iLen<2){return;}
	int iItemID=(int)p_pucData[1]-48;
	bool bEquipped=((p_pucData[2]-48)==0)?false:true;

	sql::PreparedStatement *pxPS=NULL;
	try{
		pxPS=pxCon->prepareStatement("SELECT id FROM ca_inventory WHERE charid=? AND itemid=?");
		pxPS->setInt(1,m_iPlayerID); 
		pxPS->setInt(2,iItemID); 
		if(!pxPS->execute()){m_pxConnection->SetShutdown();return;}
	}
	catch (SQLException &e){
		std::cerr<<e.what()<<std::endl;
	}

	sql::ResultSet *pxRS=pxPS->getResultSet();
	int iCount=(int)pxRS->rowsCount();
	if (iCount<=0){return;} //maybe i should notify the client that it tried to perform an ilegal action?

	

	delete pxPS;
	delete pxRS;

	pxDB->Reconnect();
	pxCon=pxDB->GetConnection();

	try{
		pxPS=pxCon->prepareStatement("UPDATE ca_inventory SET equipped=? WHERE itemid=?");
		pxPS->setBoolean(1,bEquipped); 
		pxPS->setInt(2,iItemID); 
		pxPS->executeUpdate();
	}
	catch (SQLException &e){
		std::cerr<<e.what()<<std::endl;
	}

	std::list<Item*>::iterator it;
	for(it=m_xInventory.begin();it!=m_xInventory.end();++it){
		if((*it)->ItemID()==iItemID&&(*it)->OwnerID()==m_iEntityID){
			(*it)->Equip(bEquipped);
			(*it)->SetNetChanged();
			break;
		}
	}

}
void Player::FetchInventory(){
	if(!m_pxConnection){return;}
	MySQLDB *pxDB=MySQLDB::Instance();
	pxDB->Reconnect();
	sql::Connection *pxCon=pxDB->GetConnection();

	sql::PreparedStatement *pxPS=NULL;
	try{
		pxPS=pxCon->prepareStatement("SELECT i.charid, i.itemid, i.equipped, i.inventoryidx, it.mods, it.src, bi.def, bi.mindmg, bi.maxdmg, bi.ats, bi.crit, bi.lvlreq, bi.art, bi.type FROM ca_inventory AS i LEFT JOIN ca_item AS it ON i.itemid=it.id LEFT JOIN ca_baseitem AS bi ON it.base=bi.id WHERE charid=?");
		pxPS->setInt(1,m_iPlayerID); 
		if(!pxPS->execute()){m_pxConnection->SetShutdown();return;}
	}
	catch (SQLException &e){
		std::cerr<<e.what()<<std::endl;
	}

	sql::ResultSet *pxRS=pxPS->getResultSet();
	std::stringstream xSS;
	while(pxRS->next()){
		Item *pxItem=new Item(pxRS,m_iEntityID);
		xSS<<(*pxItem);
		m_xInventory.push_back(pxItem);
	}
	int iLen=xSS.str().length();
	if(iLen<=0){return;}

	WS::WSApplicationDataFrame *pxAppFrame=new WS::WSApplicationDataFrame((unsigned char*)xSS.str().c_str(),iLen,WS::APP_PLAYER_FETCH_INVENTORY);
	WS::WSFrame *pxWSFrame=new WS::WSFrame(pxAppFrame->Frame(),pxAppFrame->FrameSize(),WS::OP_BINARY);
	m_pxConnection->Send(pxWSFrame->Data(),pxWSFrame->FrameSize());

	delete pxAppFrame;
	delete pxWSFrame;

	delete pxPS;
	delete pxRS;
}
void Player::ParseInputBuffer(unsigned char *p_pucData,int p_iLen){
	if(p_pucData==NULL){return;}
	size_t iLen=p_iLen;
	bool bC=true;
	int i,iC=(int)iLen;
	for(i=1;i<iC;i++){
		unsigned char ucK=p_pucData[i];
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
		if(ucK==LMDOWN){
			SolvePath(p_pucData+(i+1));
			i+=4;
		}
	}
<<<<<<< HEAD
	
	Cager::Zone *pxZone=m_pxInstance->GetZone();
	if(!pxZone->Collision(&xTmp)){
		m_xAABB=xTmp;
	}

	//m_bNetChanged=bC;
=======
	m_bNetChanged=bC;
>>>>>>> parent of fa8c75e... Entities, collisions and movement
}
void Player::SolvePath(unsigned char *p_pucData){
	int iIdx=0;
	
 	//int iLow=(p_pucData[iIdx++]);
	int iHigh=p_pucData[iIdx++];
	int iLow=p_pucData[iIdx++];
	int iX=(iHigh<<8)|(iLow);
	iHigh=(p_pucData[iIdx++]);
	iLow=(p_pucData[iIdx++]);
	int iY=(iHigh<<8)|(iLow);
	Vec2 vEnd(iX,iY);
	Cager::Zone *pxZone=m_pxInstance->GetZone();

	if(m_pxPath!=NULL){
		delete m_pxPath;
		m_pxPath=NULL;
	}

	m_pxPath=pxZone->SolvePath(m_xAABB.m_vOrg,vEnd);




	/*delete pxPath;
	pxPath=NULL;*/
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

	if(iCount<=0){
		delete pxPS;
		delete pxRS;
	
		unsigned char *pucReturn=new unsigned char[7];
		strcpy((char*)pucReturn,"failed");
		pucReturn[6]=0;

		WS::WSApplicationDataFrame *pxAppFrame=new WS::WSApplicationDataFrame(pucReturn,7,WS::APP_LOGIN);
		WS::WSFrame *pxWSFrame=new WS::WSFrame(pxAppFrame->Frame(),pxAppFrame->FrameSize(),WS::OP_BINARY);
		m_pxConnection->Send(pxWSFrame->Data(),pxWSFrame->FrameSize());

		delete pxAppFrame;
		delete pxWSFrame;
		return;
	}
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

	FetchInventory();
}

void Player::PreDisconnect(){
	if(!m_pxConnection){return;}

	std::cout<<"Connection closed: ";
	m_pxConnection->GetRemoteAddr().Dump();
	m_pxConnection->Close();
}


std::ostream & Player::Serialize(std::ostream & p_xSS, bool p_bFull){
	p_xSS<<m_ucEntityType;
	//BaseEntity & xBase(p_xP);
	//p_xSS<<xBase;
	BaseEntity::Serialize(p_xSS);
	p_xSS<<m_xAABB.m_vOrg;

	unsigned char ucNumItemChanged=0;
	std::list<Item*>::iterator it;
	for(it=m_xInventory.begin();it!=m_xInventory.end();++it){
		if((*it)->NetChanged()||p_bFull){
			ucNumItemChanged++;
		}
	}
	
	p_xSS<<ucNumItemChanged;
	for(it=m_xInventory.begin();it!=m_xInventory.end();++it){
		if((*it)->NetChanged()||p_bFull){
			p_xSS<<(*(*it));
			(*it)->NetClear();
		}
	}

	return p_xSS;
}

std::ostream & operator<<(std::ostream & p_xSS, Player & p_xP){
	p_xSS<<p_xP.m_ucEntityType;
	BaseEntity & xBase(p_xP);
	p_xSS<<xBase;
<<<<<<< HEAD
	p_xSS<<p_xP.m_xAABB.m_vOrg;

	unsigned char ucNumItemChanged=0;
	std::list<Item*>::iterator it;
	for(it=p_xP.m_xInventory.begin();it!=p_xP.m_xInventory.end();++it){
		if((*it)->NetChanged()){
			ucNumItemChanged++;
		}
	}
	
	p_xSS<<ucNumItemChanged;
	for(it=p_xP.m_xInventory.begin();it!=p_xP.m_xInventory.end();++it){
		if((*it)->NetChanged()){
			p_xSS<<(*(*it));
			(*it)->NetClear();
		}
	}

=======
	p_xSS<<p_xP.m_vPos;
>>>>>>> parent of fa8c75e... Entities, collisions and movement
	return p_xSS;
}