#ifndef CAGERWORLD_H
#define CAGERWORLD_H

#include <list>
#include <string>
#include "WSConnection.h"
#include "CagerInstance.h"
#include "Player.h"

class CagerWorld
{
    public:
        static CagerWorld *Instance();
        virtual ~CagerWorld();

        WS::WSConnection *NewConnection();

        void Update();
       
		void SpawnInstance();
    protected:
    private:
        CagerWorld();

        static CagerWorld *m_pxInstance;
		std::list<CagerInstance*> m_xInstances;
};

#endif // CAGERWORLD_H
