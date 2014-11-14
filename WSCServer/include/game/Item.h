#pragma once
#include "BaseEntity.h"
#include <string>
#include <prepared_statement.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <ostream>

class Item : public BaseEntity
{
public:
	Item(int p_iOwnerID);
	Item(sql::ResultSet *p_pxRes,int p_iOwnerID);
	virtual ~Item(void);

	void ParseSQLResultSet(sql::ResultSet *p_pxRes);

	friend std::ostream & operator<<(std::ostream & p_xOS, Item &p_xIT);

	int ItemID()const{return m_iItemID;}
	int OwnerID()const{return m_iOwnerID;}
	void SetNetChanged(){m_bNetChanged=true;}
	void Equip(bool p_bEquip){m_bEquipped=p_bEquip;}

private:
	Item(){}
	int m_iItemID;
	int m_iType;

	bool m_bEquipped;
	
	std::string m_sJSONMods;

	unsigned char m_ucInventoryIdx;
	unsigned char m_ucSrc;
	unsigned char m_ucDef;
	unsigned char m_ucMinDmg;
	unsigned char m_ucMaxDmg;
	unsigned char m_ucATS;
	unsigned char m_ucCritP;
	unsigned char m_ucReq;
	int m_iPlayerID;
	int m_iOwnerID;
	std::string m_sBaseArt;
	
};