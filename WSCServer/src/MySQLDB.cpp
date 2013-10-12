#include "MySQLDB.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <vector>

MySQLDB *MySQLDB::ms_pxInstance=NULL;
MySQLDB *MySQLDB::Instance(){
	if(ms_pxInstance==NULL){
		ms_pxInstance=new MySQLDB();
	}
	return ms_pxInstance;
}
MySQLDB::MySQLDB(){
	std::fstream xF(MYSQL_CONFIG, std::ios_base::in);
	assert(xF.is_open());

	while(!xF.eof()){
		std::string sLine;
		std::getline(xF,sLine);
		if(sLine.length()<=0){continue;}

		boost::trim(sLine);
		if(sLine[0]=='#'){continue;}

		std::vector<std::string> xTokens;
		boost::split(xTokens, sLine, boost::is_any_of("="));

		if(xTokens.size()<=0){continue;}
		std::string sKey=xTokens[0];
		std::string sVal=xTokens[1];
		if(sKey==MYSQL_STR_HOST){
			m_sHost=sVal;
			continue;
		}
		if(sKey==MYSQL_STR_USER){
			m_sUser=sVal;
			continue;
		}
		if(sKey==MYSQL_STR_PASSWORD){
			m_sPswd=sVal;
			continue;
		}
		if(sKey==MYSQL_STR_DATABASE){
			m_sDB=sVal;
			continue;
		}
	}
	m_pxDriver=get_driver_instance();
	
	Open();

}

MySQLDB::~MySQLDB(){
	delete ms_pxInstance;
	delete m_pxConnection;
}

void MySQLDB::Open(){
	try{
		m_pxConnection=m_pxDriver->connect(m_sHost,m_sUser,m_sPswd);
		m_pxConnection->setAutoCommit(0);

		m_pxConnection->setSchema(m_sDB);

	}catch(SQLException &e){
		std::cerr<<e.what()<<std::endl;
	}
}