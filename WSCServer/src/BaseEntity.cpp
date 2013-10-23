#include "BaseEntity.h"
#include "EntityID.h"

int BaseEntity::m_iIDIncrementer=1;
BaseEntity::BaseEntity(){
	m_bNetChanged=true; //Entity was just created, so it should be transmitted
	m_xAABB.m_vOrg=Vec2(150,150);
	m_xAABB.m_vDim=Vec2(35,35);
	m_iEntityID=m_iIDIncrementer++;
	m_ucEntityType=ENT_BASE;
}

BaseEntity::~BaseEntity(){
    //dtor
}

std::ostream & operator<<(std::ostream & p_xOS, BaseEntity & p_xBE){
	unsigned char ucHigh=0;
	if(p_xBE.m_iEntityID>255){
		ucHigh=((p_xBE.m_iEntityID>>8)&0xff);
	}
	unsigned char ucLow=(p_xBE.m_iEntityID&0xff);
	p_xOS<<ucHigh<<ucLow;
	return p_xOS;
}

