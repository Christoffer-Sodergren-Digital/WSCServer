#include "CagerInstance.h"
#include "Player.h"
#include "CagerChat.h"
#include "ApplicationDataFrame.h"
#include "WSFrame.h"
#include "Zone.h"

CagerInstance::CagerInstance(){
	m_pxChat=new CagerChat();
	m_iLastUpdate=clock();
	m_iLastSnapshot=clock();
	m_pxZone=new Cager::Zone("Zones/lobby.zon");
}

CagerInstance::~CagerInstance(){
    //dtor
}

void CagerInstance::NewPlayer(Player *p_pxNewPlayer){
	m_xPlayers.push_back(p_pxNewPlayer);
	m_pxChat->Join(p_pxNewPlayer);
	p_pxNewPlayer->SetPosition(m_pxZone->SpawnPos());
}

void CagerInstance::Update(){
	clock_t iT=clock();
	clock_t iDiff=iT-m_iLastUpdate;
	if(iDiff>(CLOCKS_PER_SEC/60.0f)){
		std::list<Player*>::iterator it;
		for(it=m_xPlayers.begin();it!=m_xPlayers.end();++it){
			Player *pxPlayer=(*it);
			pxPlayer->Update();
			if(pxPlayer->ShouldShutdown()){
				it=m_xPlayers.erase(it);
				m_pxChat->Leave(pxPlayer);
				PlayerDisconnect(pxPlayer);
				pxPlayer->PreDisconnect();
				delete pxPlayer;
				if(it==m_xPlayers.end()){
					break;
				}
			}else if(pxPlayer->NeedFullSnapshot()){
				Snapshot(pxPlayer);
			}
		}
		m_iLastUpdate=iT;
	}
	iT=clock();
	iDiff=iT-m_iLastSnapshot;
	if(iDiff>(CLOCKS_PER_SEC/50.0f)){
		Snapshot();
		m_iLastSnapshot=iT;
	}
}
void CagerInstance::Snapshot(Player *p_pxPlayer, bool p_bFull){
	if(!p_pxPlayer->IsLoggedIn()){return;}
	std::stringstream xSS;
	std::list<Player*>::iterator it;
	for(it=m_xPlayers.begin();it!=m_xPlayers.end();++it){
		Player *pxPlayer=(*it);
		if(!pxPlayer->IsLoggedIn()){continue;}
		xSS<<(*pxPlayer);
	}
	m_pxZone->Serialize(xSS,true);
	int iBuffLen=xSS.str().length();
	if(iBuffLen<=0){return;}

	unsigned char *sData=new unsigned char[iBuffLen+1];
	xSS.flush();
	xSS.read((char*)sData,iBuffLen);
	sData[iBuffLen]=0;

	WS::WSApplicationDataFrame *pxDF=new WS::WSApplicationDataFrame(sData,iBuffLen,WS::APP_SNAPSHOT);
	WS::WSFrame *pxWF=new WS::WSFrame(pxDF->Frame(),pxDF->FrameSize(),WS::OP_BINARY);

	p_pxPlayer->Connection()->Send(pxWF->Data(),pxWF->FrameSize());

	delete pxDF;
	delete pxWF;
	p_pxPlayer->SetFullSnapshot(false);
}
void CagerInstance::Snapshot(){
	std::stringstream xSS;
	std::list<Player*>::iterator it;
	for(it=m_xPlayers.begin();it!=m_xPlayers.end();++it){
		Player *pxPlayer=(*it);
		if(!pxPlayer->IsLoggedIn()){continue;}
		if(pxPlayer->NetChanged()){
			xSS<<(*pxPlayer);
			pxPlayer->NetClear();
		}
	}
	m_pxZone->Serialize(xSS);
	int iBuffLen=xSS.str().length();
	if(iBuffLen<=0){return;}
	WS::WSApplicationDataFrame *pxDF=new WS::WSApplicationDataFrame((unsigned char *)xSS.str().c_str(),iBuffLen,WS::APP_SNAPSHOT);
	WS::WSFrame *pxWF=new WS::WSFrame(pxDF->Frame(),pxDF->FrameSize(),WS::OP_BINARY);
	for(it=m_xPlayers.begin();it!=m_xPlayers.end();++it){
		Player *pxPlayer=(*it);
		if(!pxPlayer->IsLoggedIn()||pxPlayer->NeedFullSnapshot()){continue;} //don't send data to connections that are not yet logged in
		WS::WSConnection *pxCon=pxPlayer->Connection();
		pxCon->Send(pxWF->Data(),pxWF->FrameSize());
	}
	delete pxDF;
	delete pxWF;
}

void CagerInstance::PlayerDisconnect(Player *p_pxPlayer){
	if(!p_pxPlayer->IsLoggedIn()){return;}
	std::stringstream xSS;
	p_pxPlayer->DisconnectData(xSS);

	int iBuffLen=xSS.str().length();
	if(iBuffLen<=0){return;}

	WS::WSApplicationDataFrame *pxDF=new WS::WSApplicationDataFrame((unsigned char *)xSS.str().c_str(),iBuffLen,WS::APP_PLAYER_DISCONNECT);
	WS::WSFrame *pxWF=new WS::WSFrame(pxDF->Frame(),pxDF->FrameSize(),WS::OP_BINARY);

	std::list<Player*>::iterator it;
	for(it=m_xPlayers.begin();it!=m_xPlayers.end();++it){
		Player *pxPlayer=(*it);
		if(!pxPlayer->IsLoggedIn()){continue;}
		pxPlayer->Connection()->Send(pxWF->Data(),pxWF->FrameSize());
	}
}