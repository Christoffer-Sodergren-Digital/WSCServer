#include <fstream>
#include <sstream>
#include "..\include\game\Zone.h"
#include "BaseEntity.h"
#include "EntityFactory.h"

namespace Cager{

Zone::Zone(std::string p_Zone){
	m_iSpawnX=0;
	m_iSpawnY=0;
	Load(p_Zone);
}


Zone::~Zone(void){
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
		if(m_pxMap){delete m_pxMap;}
		m_pxMap=new CagerMap(4235);
		if(m_sMapSrc=="generated"){
			
		}else{
			unsigned int uiMapW,uiMapH;
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

					if(i+UP<0){
						iCount+=1;
					}else if(pucMap[i+UP]>10){
						iCount+=1;
					}
					if(i+RIGHT>=iHigh){
						iCount+=2;
					}else if(pucMap[i+RIGHT]>10){
						iCount+=2;
					}
					if(i+DOWN>=uiMapW*uiMapH*4){
						iCount+=4;
					}else if(pucMap[i+DOWN]>10){
						iCount+=4;
					}
					if(i+LEFT<iLow){
						iCount+=8;
					}else if(pucMap[i+LEFT]>10){
						iCount+=8;
					}

					if(iCount>0&&iCount<15){
						m_xCollisionBoxes.push_back(new AABB(iX*80,iY*80,80,80));
					}
				}
				iX++;
				if(iX%uiMapW==0&&iX!=0){
					iX=0;
					iY++;
				}
					
				
			}
			
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