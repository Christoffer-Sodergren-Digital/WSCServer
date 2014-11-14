#include "BaseEntity.h"
#include "EntityID.h"

int BaseEntity::m_iIDIncrementer=1;
BaseEntity::BaseEntity(){
	m_bNetChanged=true; //Entity was just created, so it should be transmitted
<<<<<<< HEAD
	m_xAABB.m_vOrg=Vec2(150,150);
	m_xAABB.m_vDim=Vec2(40,80);
=======
	m_vPos.m_fX=250.65;
	m_vPos.m_fY=250.3;
>>>>>>> parent of fa8c75e... Entities, collisions and movement
	m_iEntityID=m_iIDIncrementer++;
	m_ucEntityType=ENT_BASE;
}

BaseEntity::~BaseEntity(){
    //dtor
}

std::ostream & operator<<(std::ostream & p_xOS, BaseEntity & p_xBE){
	unsigned char ucHigh=0;
	if(p_xBE.m_iEntityID>255){
		ucHigh=((p_xBE.m_iEntityID>>4)&0xff);
	}
	unsigned char ucLow=(p_xBE.m_iEntityID&0xff);
	p_xOS<<ucHigh<<ucLow;
	return p_xOS;
}
