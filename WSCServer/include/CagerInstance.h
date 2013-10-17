#ifndef CAGERINSTANCE_H
#define CAGERINSTANCE_H

#include <list>
#include <ctime>

class CagerChat;
class Player;

enum InstanceFlag{
	INSTANCE_MAX_POPULATION=40
};

class CagerInstance
{
    public:
        CagerInstance();
        virtual ~CagerInstance();

		void Update();

		void NewPlayer(Player *p_pxNewPlayer);

		CagerChat *Chat(){return m_pxChat;}
		int Population()const{return (int)m_xPlayers.size();}

		void Snapshot();
		void Snapshot(Player *pxPlayer,bool p_bFull=true);
    protected:
    private:

	clock_t m_iLastUpdate;
	clock_t m_iLastSnapshot;
	CagerChat *m_pxChat;
    std::list<Player*> m_xPlayers;

};

#endif // CAGERINSTANCE_H
