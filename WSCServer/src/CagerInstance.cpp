#include "CagerInstance.h"
#include "Player.h"
#include "CagerChat.h"

CagerInstance::CagerInstance()
{
	m_pxChat=new CagerChat();
}

CagerInstance::~CagerInstance()
{
    //dtor
}

void CagerInstance::NewPlayer(Player *p_pxNewPlayer){
	m_xPlayers.push_back(p_pxNewPlayer);
	m_pxChat->Join(p_pxNewPlayer);
}

void CagerInstance::Update(){
	std::list<Player*>::iterator it;
	for(it=m_xPlayers.begin();it!=m_xPlayers.end();++it){
		Player *pxPlayer=(*it);
		pxPlayer->Update();
		if(pxPlayer->ShouldShutdown()){
			it=m_xPlayers.erase(it);
			m_pxChat->Leave(pxPlayer);
			delete pxPlayer;
			if(it==m_xPlayers.end()){
				break;
			}
		}
	}
}