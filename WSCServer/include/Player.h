#pragma once
#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <sstream>
#include <ctime>
#include <prepared_statement.h>
#include "BaseEntity.h"
#include "WSConnection.h"
#include "ApplicationDataFrame.h"
#include "CagerInstance.h"
#include "HiResTimer.h"

using namespace WS;

class Item;

enum KeyboardBufferKey{
	DOWN=40,
	LEFT=37,
	UP=38,
	RIGHT=39,
	LMDOWN=1,
	RMDOWN=3
};

class Player : public BaseEntity{
	public:
		Player(WSConnection *p_pxConnection,CagerInstance *p_pxInstance);
		virtual ~Player();

		WSConnection *Connection() const {return m_pxConnection;}

		inline bool ShouldShutdown()const{if(!m_pxConnection){return true;}return m_pxConnection->Shutdown();}
		inline bool IsLoggedIn()const{return m_bLoggedIn;}
		inline bool NeedFullSnapshot()const{return m_bNeedFullSnapshot;}
		inline void SetFullSnapshot(bool p_bS){m_bNeedFullSnapshot=p_bS;}

		void Update(clock_t p_iDiff);
		void AttemptLogin(unsigned char *p_pucData);

		friend std::ostream & operator<<(std::ostream & p_xSS,Player & p_xP);
		std::string & Name(){return m_sName;}


		void Player::DisconnectData(std::ostream & p_xSS);

		virtual std::ostream & Serialize(std::ostream & p_xO, bool p_bFull);
		virtual void PreDisconnect();
	protected:
	private:
		Player();
		void HandleFrame(unsigned char *p_pucData,int p_iSize);
		void ParseInputBuffer(unsigned char *p_pucData, int p_iLen);
		void FetchInventory();
		void EquipItem(unsigned char *p_pucData, int p_iLen);
		void SolvePath(unsigned char *p_pucData);
		void InterpolatePosition(clock_t p_iDiff);


		bool m_bLoggedIn;
		bool m_bNeedFullSnapshot;

		int m_iPlayerID;

		float m_fLerpT;
		float m_fLerpSpeed;

		clock_t m_iLastUpdate;

		std::string m_sName;

		std::list<Item*> m_xInventory;
		std::list<Vec2> *m_pxPath;

		CagerInstance *m_pxInstance;
		WSConnection *m_pxConnection;

};



#endif