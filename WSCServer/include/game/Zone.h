#pragma once
#include <string>
#include <list>
#include <vector>
#include <ostream>
#include "Vec2.h"
#include "BaseEntity.h"
#include "CagerMap.h"

namespace Cager{

class AStarNode{
public:
	AStarNode(){}
	AStarNode(int p_iC,int p_iR,int p_iG,int p_iH,bool p_bTraversable,AStarNode *p_pxParent=NULL);
	~AStarNode(){}

	void Set(int p_iC,int p_iR,int p_iG,int p_iH,bool p_bTraversable,AStarNode *p_pxParent=NULL);

	void SetCol(int p_iC){m_iCol=p_iC;}
	void SetRow(int p_iR){m_iRow=p_iR;}
	void SetG(int p_iG){m_iG=p_iG;}
	void SetH(int p_iH){m_iHer=p_iH;}
	void SetParent(AStarNode *p_pxParent){m_pxParent=p_pxParent;}

	int GetCol(){return m_iCol;}
	int GetRow(){return m_iRow;}
	int GetG(){return m_iG;}
	int GetH(){return m_iHer;}
	bool IsTraversable()const{return m_bTraversable;}
	AStarNode *GetParent(){return m_pxParent;}

	int CalculateG() const;
	void Reset();

private:
	int m_iCol;
	int m_iRow;
	int m_iG;
	int m_iHer;
	bool m_bTraversable;
	AStarNode *m_pxParent;
		 
};

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
	std::list<Vec2> *SolvePath(Vec2 p_Start, Vec2 p_End);
	
	std::ostream & Serialize(std::ostream & p_xOS, bool p_bFull=false);
	std::list<BaseEntity*> & Entities(){return m_xEntities;}

private:

	void LoadMapFromFile();
	void GenerateMap();

	AStarNode **GetAdjecent(AStarNode *p_pxP);
	AStarNode *GetNodeFromColRow(int p_iC,int p_iR);
	AStarNode *GetNodeFromWorldPos(Vec2 p_vPos);

	int m_iSpawnX;
	int m_iSpawnY;
	int m_iTileS;
	std::string m_sMapSrc;
	std::vector<AStarNode*> m_xPathingData;
	std::list<BaseEntity*> m_xEntities;
	std::list<AABB*> m_xCollisionBoxes;
	CagerMap *m_pxMap;
};


}