#pragma once

#include <string>

class BaseEntity;

class EntityFactory
{
public:
	EntityFactory(void){}
	~EntityFactory(void){}

	BaseEntity *Construct(std::string & p_sClassName);
};

