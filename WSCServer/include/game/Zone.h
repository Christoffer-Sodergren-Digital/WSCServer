#pragma once
#include <string>
#include <list>
#include <ostream>
#include "Vec2.h"
#include "BaseEntity.h"
#include "CagerMap.h"

namespace Cager{

class Zone
{
public:
	Zone(std::string p_sFilename);
	~Zone(void);

	void Load(std::string p_sFilename);

	int SpawnX(){return m_iSpawnX;}
	int SpawnY(){return m_iSpawnY;}
	Vec2 SpawnPos(){return Vec2(m_iSpawnX, m_iSpawnY);}

	bool Collision(AABB *p_pxAABB);
	
	std::ostream & Serialize(std::ostream & p_xOS, bool p_bFull=false);
	std::list<BaseEntity*> & Entities(){return m_xEntities;}

private:
	int m_iSpawnX;
	int m_iSpawnY;
	std::string m_sMapSrc;
	std::list<BaseEntity*> m_xEntities;
	std::list<AABB*> m_xCollisionBoxes;
	CagerMap *m_pxMap;
};


}