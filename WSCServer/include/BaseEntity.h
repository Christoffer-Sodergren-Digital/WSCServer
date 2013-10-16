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

       // friend std::ostream & Serialize(std::ostream & p_xOs, const BaseEntity & p_xEnt);
    protected:
		Vec2 m_vPos;
		EntityDescription *m_pxEntityDescription;
    private:
        
		
};

#endif // BASEENTITY_H
