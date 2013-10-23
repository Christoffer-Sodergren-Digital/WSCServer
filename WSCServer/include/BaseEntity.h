#ifndef BASEENTITY_H
#define BASEENTITY_H

#include <sstream>
#include "Vec2.h"

class EntityDescription;

class AABB{
public:
	AABB(){}
	AABB(float p_fX,float p_fY,float p_fW,float p_fH){m_vOrg=Vec2(p_fX,p_fY);m_vDim=Vec2(p_fW,p_fH);}
	~AABB(){}
	Vec2 m_vOrg;
	Vec2 m_vDim;
	bool HitTest(AABB *p_xAABB){
		Vec2 v0=p_xAABB->m_vOrg;
		Vec2 d0=p_xAABB->m_vDim;
		if(m_vOrg.m_fX+m_vDim.m_fX<v0.m_fX||m_vOrg.m_fX>v0.m_fX+d0.m_fX){
			return false;
		}
		if(m_vOrg.m_fY+m_vDim.m_fY<v0.m_fY||m_vOrg.m_fY>v0.m_fY+d0.m_fY){
			return false;
		}
		return true;
	}
};

class BaseEntity
{
    public:
        BaseEntity();
        virtual ~BaseEntity();

		
		bool NetChanged()const{return m_bNetChanged;}
		void NetClear(){m_bNetChanged=false;}

		void SetPosition(Vec2 p_vV){m_xAABB.m_vOrg=p_vV;}

		virtual void Interact(BaseEntity *p_pxInteractee){}

		virtual void DeSerialize(std::istream & p_xIS){}
		virtual std::ostream & Serialize(std::ostream & p_xOS){p_xOS<<(*this);return p_xOS;}

		virtual void PreDisconnect(){}

		friend std::ostream & operator<<(std::ostream & p_xSS, BaseEntity & p_xEnt);
    protected:
		//Vec2 m_vPos;
		AABB m_xAABB;
		static int m_iIDIncrementer;
		int m_iEntityID;
		bool m_bNetChanged;
		unsigned char m_ucEntityType;
    private:
		
};


#endif // BASEENTITY_H
