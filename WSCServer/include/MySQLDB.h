#ifndef MYSQL_DB
#define MYSQL_DB

#define MYSQL_CONFIG "MySql/mysql_config.txt"
#define MYSQL_STR_HOST "HOST"
#define MYSQL_STR_USER "USER"
#define MYSQL_STR_PASSWORD "PASSWORD"
#define MYSQL_STR_DATABASE "DATABASE"

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>

using namespace sql;

class MySQLDB{
public:
	static MySQLDB *Instance();
	virtual ~MySQLDB();

	Connection *GetConnection(){return m_pxConnection;}

protected:
private:
	MySQLDB();

	void Open();

	Driver *m_pxDriver;
	Connection *m_pxConnection;

	std::string m_sHost;
	std::string m_sUser;
	std::string m_sPswd;
	std::string m_sDB;

	static MySQLDB *ms_pxInstance;

};


#endif