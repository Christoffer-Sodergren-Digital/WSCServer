// WSCServer.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <cstdlib>
//#include <mysql_connection.h>
//#include <mysql_driver.h>
//#include <cppconn/exception.h>
//#include <cppconn/statement.h>
#include "TcpSocket.h"
#include "StringHelper.h"
#include "ResourceManager.h"
#include "CagerWorld.h"
#include "WSConnection.h"
#include "MySQLDB.h"


using namespace WS;


int main(int argc,char **argv){

	//sql::Driver *pxDriver;
	//sql::Connection *pxCon;

	//pxDriver = sql::mysql::get_driver_instance();
	//
	//try{
	//	pxCon=pxDriver->connect("127.0.0.1","cager","asd");
	//	pxCon->setAutoCommit(0);

	//	pxCon->setSchema("cager");
	//	sql::ResultSet *pxRes;
	//	sql::Statement *pxStmnt=pxCon->createStatement();
	//	/*sql::SQLString sQuery("SELECT name FROM ca_user");*/
	//	pxRes=pxStmnt->executeQuery("SELECT name FROM ca_user");
	//	while(pxRes->next()){
	//		sql::SQLString xStr=pxRes->getString(1);
	//		std::cout<<xStr<<std::endl;
	//	}


	//}catch(sql::SQLException &e){
	//		std::cout<<e.what();
	//}

    ResourceManager *pxInstance=ResourceManager::Instance();
    CagerWorld *pxWorld=CagerWorld::Instance();

    WORD wVersion;
    WSAData xWSAData;

    wVersion=MAKEWORD(1,1);

    WSAStartup(wVersion,&xWSAData);

    TCPAcceptSocket xListen;
    IPAddr4 xAddr;
    xAddr.sin_family=AF_INET;
    xAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    xAddr.sin_port=htons(10444);
    xListen.Open(xAddr,5,3);

    if(xListen.IsOpen()){
        std::cout<<"Listening on interface 0.0.0.0 port: 10444\n";
        bool bDone=false;
        while(!bDone){
            if(xListen.ConnectionPending(10)){
                WSConnection *pxNewCon=pxWorld->NewConnection();
                if(!xListen.Accept(*pxNewCon)){
                    //pxWorld->RemoveConnection(pxNewCon);
                }else{
                    std::cout<<"New connection: ";
                    pxNewCon->GetRemoteAddr().Dump();
                }
            }
            pxWorld->Update();
        }
    }

   std::cin.get();
}



