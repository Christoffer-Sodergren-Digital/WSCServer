#include <fstream>
#include <sstream>
#include <assert.h>
#include <cmath>
#include "..\include\game\Zone.h"
#include "BaseEntity.h"
#include "EntityFactory.h"

namespace Cager{

AStarNode::AStarNode(int p_iC,int p_iR,int p_iG,int p_iH,bool p_bTraversable,AStarNode *p_pxParent){
	m_iCol=p_iC;
	m_iRow=p_iR;
	m_iG=p_iG;
	m_iHer=p_iH;
	m_bTraversable=p_bTraversable;
	m_pxParent=p_pxParent;
}

void AStarNode::Set(int p_iC,int p_iR,int p_iG,int p_iH,bool p_bTraversable,AStarNode *p_pxParent){
	m_iCol=p_iC;
	m_iRow=p_iR;
	m_iG=p_iG;
	m_iHer=p_iH;
	m_bTraversable=p_bTraversable;
	m_pxParent=p_pxParent;
}

void AStarNode::Reset(){
	m_iG=0;
	m_iHer=0;
	m_pxParent=NULL;
}

Zone::Zone(std::string p_Zone){
	m_iSpawnX=0;
	m_iSpawnY=0;
	m_iTileS=80;
	Load(p_Zone);
}

Zone::~Zone(void){
	std::list<AABB*>::iterator it;
	for(it=m_xCollisionBoxes.begin();it!=m_xCollisionBoxes.end();++it){
		delete (*it);
	}
	std::vector<AStarNode*>::iterator nodeIt;
	for(nodeIt=m_xPathingData.begin();nodeIt!=m_xPathingData.end();++nodeIt){
			delete (*nodeIt);
	}
}

void Zone::Load(std::string p_Zone){
	std::fstream xFS(p_Zone.c_str(),std::ios_base::in);
	if(!xFS.is_open()){
		return;
	}
	EntityFactory xEF;
	std::string sTmp;
	while(!xFS.eof()){
		std::getline(xFS,sTmp);
		if(sTmp.length()<0||sTmp[0]=='#'){continue;}
		int iPos=sTmp.find('=');
		std::string sVar=sTmp.substr(0,iPos);

		if(sVar.compare("spawnx")==0){
			std::stringstream xSS;
			xSS<<sTmp.substr(iPos+1);
			xSS>>m_iSpawnX;
		}
		if(sVar.compare("spawny")==0){
			std::stringstream xSS;
			xSS<<sTmp.substr(iPos+1);
			xSS>>m_iSpawnY;
		}
		if(sVar.compare("map")==0){
			m_sMapSrc=sTmp.substr(iPos+1);
		}
		if(sVar.compare("entities")==0){
			std::stringstream xSS;
			xSS<<sTmp.substr(iPos+1);
			int iNumEnts=0;
			xSS>>std::skipws>>iNumEnts;
			int i,iC=iNumEnts;
			for(i=0;i<iC;i++){
				std::string sClassName;
				xSS>>std::skipws>>sClassName;
				BaseEntity *pxEnt=xEF.Construct(sClassName);
				pxEnt->DeSerialize(xSS);
				m_xEntities.push_back(pxEnt);
			}
		}
	}
	if(m_sMapSrc.length()>0){
		if(m_sMapSrc=="generated"){
			GenerateMap();
		}else{
			LoadMapFromFile();
		}
		
	}
}

void Zone::GenerateMap(){
	if(m_pxMap){delete m_pxMap;}
	m_pxMap=new CagerMap(4235);
}

AStarNode *Zone::GetNodeFromColRow(int p_iC,int p_iR){
	int i,iC=m_xPathingData.size();
	for(i=0;i<iC;i++){
		AStarNode *pxP=m_xPathingData[i];
		if(pxP->GetCol()==p_iC&&pxP->GetRow()==p_iR){
			return pxP;
		}
	}
	return NULL;
}
AStarNode *Zone::GetNodeFromWorldPos(Vec2 p_vPos){
	int i,iC=m_xPathingData.size();
	for(i=0;i<iC;i++){
		AStarNode *pxP=m_xPathingData[i];
		int iTileX=(pxP->GetCol()*m_iTileS);
		int iTileY=(pxP->GetRow()*m_iTileS);
		if(p_vPos.m_fX>=iTileX&&p_vPos.m_fX<=iTileX+m_iTileS){
			if(p_vPos.m_fY>=iTileY&&p_vPos.m_fY<=iTileY+m_iTileS){
				return pxP;
			}
		}
	}
	return NULL;
}
AStarNode **Zone::GetAdjecent(AStarNode *p_xP){
	int iIdx=-1;
	int i,iC=m_xPathingData.size();
	for(i=0;i<iC;i++){
		if(m_xPathingData[i]==p_xP){
			iIdx=i;
			break;
		}
	}
	if(iIdx==-1){return NULL;}
	AStarNode **pxRet=new AStarNode*[8];

	int iRetIdx=0;
	
	pxRet[iRetIdx++]=GetNodeFromColRow(p_xP->GetCol()-1,p_xP->GetRow());
	pxRet[iRetIdx++]=GetNodeFromColRow(p_xP->GetCol()+1,p_xP->GetRow());
	pxRet[iRetIdx++]=GetNodeFromColRow(p_xP->GetCol(),p_xP->GetRow()-1);
	pxRet[iRetIdx++]=GetNodeFromColRow(p_xP->GetCol(),p_xP->GetRow()+1);

	pxRet[iRetIdx++]=GetNodeFromColRow(p_xP->GetCol()-1,p_xP->GetRow()-1);
	pxRet[iRetIdx++]=GetNodeFromColRow(p_xP->GetCol()+1,p_xP->GetRow()-1);
	pxRet[iRetIdx++]=GetNodeFromColRow(p_xP->GetCol()-1,p_xP->GetRow()+1);
	pxRet[iRetIdx++]=GetNodeFromColRow(p_xP->GetCol()+1,p_xP->GetRow()+1);
	
	return pxRet;
}

std::list<Vec2> *Zone::SolvePath(Vec2 p_vStart, Vec2 p_vEnd){
	std::vector<AStarNode*> xOpen;
	std::vector<AStarNode*> xClosed;
	std::list<Vec2> *pxRet=new std::list<Vec2>();
	std::vector<AStarNode*>::iterator it;

	for(it=m_xPathingData.begin();it!=m_xPathingData.end();++it){
		(*it)->Reset();
	}
	AStarNode *pxCurrent=NULL;

	AStarNode *pxStart=GetNodeFromWorldPos(p_vStart);
	AStarNode *pxEnd=GetNodeFromWorldPos(p_vEnd);
	if(pxStart==NULL){return pxRet;}
	if(pxEnd==NULL){return pxRet;}
	if(!pxEnd->IsTraversable()){
		return pxRet;
	}
	xOpen.push_back(pxStart);
	int iSanity=100000;
	while(xOpen.size()>0&&iSanity-->0){
		int i,iC=xOpen.size();
		
		int iMinF=10000000;
		for(i=0;i<iC;i++){
			AStarNode *pxTmp=xOpen[i];
			int iG=pxTmp->GetG();
			int iH=pxTmp->GetH();
			if(iG+iH<iMinF){
				pxCurrent=pxTmp;
				iMinF=iG+iH;
			}
		}
		if(pxCurrent==NULL){return pxRet;}
		if(pxCurrent==pxEnd){break;}
		AStarNode **pxAdj=GetAdjecent(pxCurrent);
		iC=8;
		for(i=0;i<iC;i++){
			AStarNode *pxComp=pxAdj[i];
			if(pxComp==NULL){continue;}
			std::vector<AStarNode*>::iterator xInClosed=std::find(xClosed.begin(),xClosed.end(),pxComp);
			if(xInClosed!=xClosed.end()){continue;}

			std::vector<AStarNode*>::iterator it=std::find(xOpen.begin(),xOpen.end(),pxComp);
			if(it!=xOpen.end()){
				if((*it)->GetG()>pxCurrent->GetG()+pxComp->GetG()){
					(*it)->SetParent(pxCurrent);
				}
			}else{
				pxComp->SetParent(pxCurrent);
				pxComp->SetG((i<4)?10:14);
				int iDX=abs((pxEnd->GetCol())-(pxComp->GetCol()));
				int iDY=abs((pxEnd->GetRow())-(pxComp->GetRow()));
				pxComp->SetH(iDX+iDY);

				xOpen.push_back(pxComp);
			}

		}
		xClosed.push_back(pxCurrent);
		xOpen.erase(std::find(xOpen.begin(),xOpen.end(),pxCurrent));
		delete [] pxAdj;
	}
	if(pxCurrent==pxEnd){
		pxRet->push_back(Vec2(pxEnd->GetCol()*m_iTileS,pxEnd->GetRow()*m_iTileS));
		AStarNode *pxCurr=xClosed.back();
		int iSanity=10000;
		while(pxCurr->GetParent()!=NULL&&iSanity-->0){
			pxRet->push_back(Vec2(pxCurr->GetCol()*m_iTileS,pxCurr->GetRow()*m_iTileS));
			pxCurr=pxCurr->GetParent();
		}
	}

	return pxRet;
}

void Zone::LoadMapFromFile(){
	unsigned int uiMapW,uiMapH;
	if(m_pxMap){delete m_pxMap;}
	m_pxMap=new CagerMap(4235);
	m_pxMap->GenerateFromPNG(m_sMapSrc,uiMapW,uiMapH);
	unsigned char *pucMap=m_pxMap->MapData();
	if(!pucMap){return;}
	int iX=0,iY=0;
	int iW=uiMapW*4,iH=uiMapH*4;
	int LEFT=-4,RIGHT=4,UP=-iW,DOWN=iW;
	int i, iC=uiMapW*uiMapH*4;
	for(i=0;i<iC;i+=4){
		int iCount=0;
		int iRow=i/iW;
		int iLow=iRow*iW;
		int iHigh=(iRow+1)*iW;
				
		if(pucMap[i]>0){
			if(i+UP>=0&&pucMap[i+UP]>10){
				iCount+=1;
			}
			if(i+RIGHT<iHigh&&pucMap[i+RIGHT]>10){
				iCount+=2;
			}
			if(i+DOWN<uiMapW*uiMapH*4&&pucMap[i+DOWN]>10){
				iCount+=4;
			}
			if(i+LEFT>=iLow&&pucMap[i+LEFT]>10){
				iCount+=8;
			}
		}
		bool bTraversable=(iCount==0);

		if(bTraversable){
			int a=0;
		}
		m_xPathingData.push_back(new AStarNode(iX,iY,0,0,bTraversable));

		if(iCount>0){
			m_xCollisionBoxes.push_back(new AABB(iX*80,iY*80,80,80));
		}
		
		iX++;
		if(iX%uiMapW==0&&iX!=0){
			iX=0;
			iY++;
		}
	}
}

bool Zone::Collision(AABB *p_xAABB){
	std::list<AABB*>::iterator it;
	for(it=m_xCollisionBoxes.begin();it!=m_xCollisionBoxes.end();++it){
		if((*it)->HitTest(p_xAABB)){
			return true;
		}
	}
	return false;
}

std::ostream & Zone::Serialize(std::ostream & p_xOS, bool p_bFull){
	std::list<BaseEntity*>::iterator it;
	for(it=m_xEntities.begin();it!=m_xEntities.end();++it){
		if((*it)->NetChanged()||p_bFull){
			(*it)->Serialize(p_xOS);
			if(!p_bFull){
				(*it)->NetClear();
			}
		}
	}
	return p_xOS;
}

//std::ostream & operator<<(std::ostream & p_xOS, Zone & p_xZ){
//	std::list<BaseEntity*>::iterator it;
//	for(it=p_xZ.m_xEntities.begin();it!=p_xZ.m_xEntities.end();++it){
//		if((*it)->NetChanged()){
//			(*it)->Serialize(p_xOS);
//			(*it)->NetClear();
//		}
//	}
//	return p_xOS;
//}

}