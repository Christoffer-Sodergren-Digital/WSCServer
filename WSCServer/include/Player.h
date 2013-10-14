#pragma once
#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "BaseEntity.h"
#include "WSConnection.h"
#include "ApplicationDataFrame.h"
#include "CagerInstance.h"

using namespace WS;

class Player : public BaseEntity{
	public:
		Player(WSConnection *p_pxConnection,CagerInstance *p_pxInstance);
		virtual ~Player();

		WSConnection *Connection() const {return m_pxConnection;}

		bool ShouldShutdown()const{return m_bShutdown;}

		void Update();
		void AttemptLogin(unsigned char *p_pucData);

		//std::ostream & Serialize();

		void Snapshot();
		std::string & Name(){return m_sName;}
	protected:
	private:
		Player();
		//WSApplicationDataFrame *ConstructFrame(unsigned char p_ucOpCode);
		void HandleFrame(unsigned char *p_pucData);

		bool m_bShutdown;
		bool m_bLoggedIn;

		int m_iPlayerID;

		std::string m_sName;

		CagerInstance *m_pxInstance;
		WSConnection *m_pxConnection;
};



#endif