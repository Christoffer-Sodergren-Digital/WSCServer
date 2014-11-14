#include "..\include\game\Item.h"
#include "EntityID.h"

Item::Item(int p_iOwnerID){
	m_ucEntityType=ENT_ITEM;
	m_iItemID=0;
	m_iType=0;

	m_bEquipped=false;
	
	m_sJSONMods="";

	m_ucInventoryIdx=0;
	m_ucSrc=0;
	m_ucDef=0;
	m_ucMinDmg=0;
	m_ucMaxDmg=0;
	m_ucATS=0;
	m_ucCritP=0;
	m_ucReq=0;
	m_sBaseArt="";
	m_iPlayerID=0;
	m_iOwnerID=p_iOwnerID;
}


Item::Item(sql::ResultSet *p_pxRes,int p_iOwnerID){
	m_ucEntityType=ENT_ITEM;
	m_iItemID=0;
	m_iType=0;

	m_bEquipped=false;
	
	m_sJSONMods="";

	m_ucInventoryIdx=0;
	m_ucSrc=0;
	m_ucDef=0;
	m_ucMinDmg=0;
	m_ucMaxDmg=0;
	m_ucATS=0;
	m_ucCritP=0;
	m_ucReq=0;
	m_sBaseArt="";
	m_iPlayerID=0;
	m_iOwnerID=p_iOwnerID;

	ParseSQLResultSet(p_pxRes);
}

Item::~Item(void){
}

void Item::ParseSQLResultSet(sql::ResultSet *p_pxRes){
	m_iItemID=p_pxRes->getInt("itemid");
	m_bEquipped=(bool)p_pxRes->getInt("equipped");
	m_ucInventoryIdx=p_pxRes->getInt("inventoryidx");
	m_ucSrc=p_pxRes->getInt("src");
	m_ucDef=p_pxRes->getInt("def");
	m_ucMinDmg=p_pxRes->getInt("mindmg");
	m_ucMaxDmg=p_pxRes->getInt("maxdmg");
	m_ucATS=p_pxRes->getDouble("ats");
	m_ucCritP=p_pxRes->getDouble("crit");
	m_ucReq=p_pxRes->getInt("lvlreq");
	m_iType=p_pxRes->getInt("type");
	m_sBaseArt=p_pxRes->getString("art");
	m_iPlayerID=p_pxRes->getInt("charid");
}

std::ostream & operator<<(std::ostream & p_xOS, Item &p_xIT){
	p_xOS<<p_xIT.m_ucEntityType;
	BaseEntity &xBase(p_xIT);
	p_xOS<<xBase;

	unsigned char ucHigh=(p_xIT.m_iItemID>>8);
	unsigned char ucLow=(p_xIT.m_iItemID&0xff);
	p_xOS<<ucHigh<<ucLow;

	ucHigh=(p_xIT.m_iOwnerID>>8);
	ucLow=(p_xIT.m_iOwnerID&0xff);
	p_xOS<<ucHigh<<ucLow;

	ucHigh=(p_xIT.m_iType>>8);
	ucLow=(p_xIT.m_iType&0xff);
	p_xOS<<ucHigh<<ucLow;

	unsigned char ucEquipped=(p_xIT.m_bEquipped)?1:0;
	p_xOS<<ucEquipped;

	unsigned char ucModLen=(unsigned char)p_xIT.m_sJSONMods.length();
	p_xOS<<ucModLen<<p_xIT.m_sJSONMods.c_str();

	p_xOS<<p_xIT.m_ucInventoryIdx;
	p_xOS<<p_xIT.m_ucSrc;
	p_xOS<<p_xIT.m_ucDef;
	p_xOS<<p_xIT.m_ucMaxDmg;
	p_xOS<<p_xIT.m_ucMinDmg;
	p_xOS<<p_xIT.m_ucATS;
	p_xOS<<p_xIT.m_ucCritP;
	p_xOS<<p_xIT.m_ucReq;

	assert(p_xIT.m_sBaseArt.length()<255);
	p_xOS<<(unsigned char)p_xIT.m_sBaseArt.length();
	p_xOS<<p_xIT.m_sBaseArt;

	return p_xOS;
}