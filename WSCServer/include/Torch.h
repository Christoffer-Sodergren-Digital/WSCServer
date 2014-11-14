#pragma once
#include "baseentity.h"

#include <ostream>
#include <istream>

class Torch :
	public BaseEntity
{
public:
	Torch(void);
	virtual ~Torch(void);

	virtual void Interact(BaseEntity *p_pxInteractee);

	virtual void DeSerialize(std::istream & p_xSS);
	virtual std::ostream & Serialize(std::ostream & p_xOS);

	friend std::ostream & operator<<(std::ostream & p_xSS,Torch & p_xT);
private:
	int m_iState;
};

