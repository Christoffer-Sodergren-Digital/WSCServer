#include <iostream>
#include "CagerWorld.h"
#include "CagerInstance.h"

using namespace WS;

CagerWorld *CagerWorld::m_pxInstance=0;
CagerWorld::CagerWorld()
{
	CagerInstance *pxLobby=new CagerInstance();
	m_xInstances.push_back(pxLobby);
}

CagerWorld::~CagerWorld()
{

}
CagerWorld *CagerWorld::Instance()
{
    if(m_pxInstance==0){
        m_pxInstance=new CagerWorld();
    }
    return m_pxInstance;
}

WSConnection *CagerWorld::NewConnection()
{
    WSConnection *pxNewCon=new WSConnection();
	std::list<CagerInstance *>::iterator it=m_xInstances.end();
	--it;
	CagerInstance *pxLastInstance=(*it);
	if(pxLastInstance->Population()>=INSTANCE_MAX_POPULATION){
		CagerInstance *pxNewInstance=new CagerInstance();
		m_xInstances.push_back(pxNewInstance);
		pxLastInstance=pxLastInstance;
	}
	Player *pxNewPlayer=new Player(pxNewCon,pxLastInstance);
	pxLastInstance->NewPlayer(pxNewPlayer);

    return pxNewCon;
}
void CagerWorld::Update(){

	//TODO: Multithreading
	std::list<CagerInstance*>::iterator it;
	for(it=m_xInstances.begin();it!=m_xInstances.end();++it){
		CagerInstance *pxInstance=(*it);
		if(pxInstance->Population()<=0&&m_xInstances.size()>1){
			it=m_xInstances.erase(it);
		}else{
			pxInstance->Update();
		}
	}
}

