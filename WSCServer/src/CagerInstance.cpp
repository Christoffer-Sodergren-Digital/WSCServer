#include "CagerInstance.h"
#include "Player.h"
#include "CagerChat.h"
#include "ApplicationDataFrame.h"
#include "WSFrame.h"

CagerInstance::CagerInstance(){
	m_pxChat=new CagerChat();
	m_iLastUpdate=clock();
	m_iLastSnapshot=clock();
}

CagerInstance::~CagerInstance(){
    //dtor
}

void CagerInstance::NewPlayer(Player *p_pxNewPlayer){
	m_xPlayers.push_back(p_pxNewPlayer);
	m_pxChat->Join(p_pxNewPlayer);
}

void CagerInstance::Update(){
	clock_t iT=clock();
	clock_t iDiff=iT-m_iLastUpdate;
	if(iDiff>(CLOCKS_PER_SEC/30.3f)){
		std::list<Player*>::iterator it;
		for(it=m_xPlayers.begin();it!=m_xPlayers.end();++it){
			Player *pxPlayer=(*it);
			pxPlayer->Update(iDiff);
			if(pxPlayer->ShouldShutdown()){
				it=m_xPlayers.erase(it);
				m_pxChat->Leave(pxPlayer);
				pxPlayer->PreDisconnect();
				delete pxPlayer;
				if(it==m_xPlayers.end()){
					break;
				}
			}else if(pxPlayer->NeedFullSnapshot()){
				Snapshot(pxPlayer,true);
			}
		}
		m_iLastUpdate=iT;
	}
	iT=clock();
	iDiff=iT-m_iLastSnapshot;
	if(iDiff>(CLOCKS_PER_SEC/10.0f)){
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
<<<<<<< HEAD
		if(!pxPlayer->IsLoggedIn()){continue;}
		pxPlayer->Serialize(xSS, true);
=======
		xSS<<(*pxPlayer);
>>>>>>> parent of fa8c75e... Entities, collisions and movement
	}
	int iBuffLen=xSS.str().length();
	if(iBuffLen<=0){return;}

	unsigned char *sData=new unsigned char[iBuffLen+1];
	xSS.flush();
	xSS.read((char*)sData,iBuffLen);
	sData[iBuffLen]=0;

	WS::WSApplicationDataFrame *pxDF=new WS::WSApplicationDataFrame(sData,iBuffLen,WS::APP_FULL_SNAPSHOT);
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
<<<<<<< HEAD
=======

			pxPlayer->NetClear();
>>>>>>> parent of fa8c75e... Entities, collisions and movement
		}
	}
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