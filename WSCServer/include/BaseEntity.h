#ifndef BASEENTITY_H
#define BASEENTITY_H

#include <sstream>
#include "Vec3.h"

class EntityDescription;

class BaseEntity
{
    public:
        BaseEntity();
        virtual ~BaseEntity();

        friend std::ostream & Serialize(std::ostream & p_xOs, const BaseEntity & p_xEnt);
    protected:
    private:
        EntityDescription *m_pxEntityDescription;
};

#endif // BASEENTITY_H
