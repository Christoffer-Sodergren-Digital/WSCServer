#ifndef BASEENTITY_H
#define BASEENTITY_H

#include <sstream>
#include "Vec2.h"

class EntityDescription;

class BaseEntity
{
    public:
        BaseEntity();
        virtual ~BaseEntity();

		friend std::ostream & operator<<(std::ostream & p_xSS, BaseEntity & p_xEnt);
		bool NetChanged()const{return m_bNetChanged;}
		void NetClear(){m_bNetChanged=false;}

		virtual void PreDisconnect(){}
    protected:
		Vec2 m_vPos;
		static int m_iIDIncrementer;
		int m_iEntityID;
		bool m_bNetChanged;
		unsigned char m_ucEntityType;
    private:
		
};



#endif // BASEENTITY_H
