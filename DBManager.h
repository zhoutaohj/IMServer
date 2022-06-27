#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <string>
#include <json/json.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <map>
#include "DBConnectPool.h"
#include "NoCopy.h"
#include <memory>
#include "DBQueryResult.h"
//single 

class DBManager :public NoCopy
{
public:
    
    ~DBManager();
    
    static std::shared_ptr<DBManager>  GetDBManager();
    DBQueryResult Query(const std::string & key,const std::string &);
    int Update(const std::string &);// insert update delete

private:
    DBManager(const std::string&,const std::string&,const std::string&,const std::string&);
    static void Init();
private:
    std::string _db_address;
    std::string _db_port;
    std::string _database_name;
    std::string _login_name;
    std::string _passwd;
    DBConnectPool _db_connnect_pool;

    static pthread_once_t once_var;
    static std::shared_ptr<DBManager> _dbmanager_sp;
    
};


#endif