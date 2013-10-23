#include "..\include\Torch.h"
#include "EntityID.h"

Torch::Torch(void){
	m_iState=1;
	m_ucEntityType=ENT_TORCH;
}


Torch::~Torch(void){
}

void Torch::Interact(BaseEntity *p_pxInteractee){
	m_bNetChanged=true;
	m_iState=(m_iState==0)?1:0;
}

void Torch::DeSerialize(std::istream & p_xIS){
	int iPosX,iPosY;
	p_xIS>>std::skipws>>iPosX>>iPosY>>m_iState;
	m_xAABB.m_vOrg=Vec2(iPosX,iPosY);;
}

std::ostream & Torch::Serialize(std::ostream & p_xOS){
	unsigned char ucState=(m_iState&0xff);
	
	p_xOS<<m_ucEntityType;
	BaseEntity & xBase((*this));
	p_xOS<<xBase;
	p_xOS<<ucState;
	p_xOS<<m_xAABB.m_vOrg;
	
	return p_xOS;
	
}

std::ostream & operator<<(std::ostream & p_xSS,Torch & p_xT){
	if(!p_xT.NetChanged()){return p_xSS;}
	unsigned char ucState=(p_xT.m_iState&0xff);
	
	p_xSS<<p_xT.m_ucEntityType;
	BaseEntity & xBase(p_xT);
	p_xSS<<xBase;
	p_xSS<<ucState;
	p_xSS<<p_xT.m_xAABB.m_vOrg;
	return p_xSS;
}
