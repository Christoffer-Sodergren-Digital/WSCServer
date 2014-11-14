#include "..\include\EntityFactory.h"
#include "Torch.h"
#include "BaseEntity.h"

BaseEntity *EntityFactory::Construct(std::string & p_sClassName){
	if(p_sClassName=="Torch"){
		return new Torch();
	}
}

