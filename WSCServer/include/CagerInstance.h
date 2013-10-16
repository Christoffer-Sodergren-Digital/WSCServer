#ifndef CAGERINSTANCE_H
#define CAGERINSTANCE_H

#include <list>

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
    protected:
    private:

	CagerChat *m_pxChat;
    std::list<Player*> m_xPlayers;

};

#endif // CAGERINSTANCE_H
