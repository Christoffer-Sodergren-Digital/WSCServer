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

enum KeyboardBufferKey{
	DOWN=0x01,
	LEFT=0x02,
	UP=0x03,
	RIGHT=0x04
};

class Player : public BaseEntity{
	public:
		Player(WSConnection *p_pxConnection,CagerInstance *p_pxInstance);
		virtual ~Player();

		WSConnection *Connection() const {return m_pxConnection;}

		bool ShouldShutdown()const{return m_bShutdown;}

		void Update();
		void AttemptLogin(unsigned char *p_pucData);

		friend std::stringstream & operator<<(std::stringstream & p_xSS,Player & p_xP);

		void Snapshot();
		std::string & Name(){return m_sName;}

		void PreDisconnect();
	protected:
	private:
		Player();
		//WSApplicationDataFrame *ConstructFrame(unsigned char p_ucOpCode);
		void HandleFrame(unsigned char *p_pucData);
		void ParseInputBuffer(unsigned char *p_pucData);
		void UpdateDB();

		bool m_bShutdown;
		bool m_bLoggedIn;

		int m_iPlayerID;

		clock_t m_iLastUpdate;


		std::string m_sName;

		CagerInstance *m_pxInstance;
		WSConnection *m_pxConnection;

};



#endif