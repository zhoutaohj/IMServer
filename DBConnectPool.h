#ifndef DBCONNECTPOOL_H
#define DBCONNECTPOOL_H
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <vector>
#include <list>
#include <memory>
#include "Mutex.h"

class DBManager;
class DBConnectPool
{
    friend class DBManager;
public:
    DBConnectPool(const std::string& ip,const std::string& port,const std::string& name,const std::string& passwd);
    ~DBConnectPool();

private:
    void InitPool(size_t connect_count=4);
    std::shared_ptr<sql::Connection> GetConnnect();
    void ReleaseConnnect(std::shared_ptr<sql::Connection>);
    void  CreateConnnect();
    void  SetDataBase();
  
private : 
    sql::Driver* driver_ptr;

    Mutex _mutex;
    std::list<std::shared_ptr<sql::Connection>> idle_connect_list;
    std::list<std::shared_ptr<sql::Connection>> busy_connect_list;

    std::string _db_address;
    std::string _db_port;
    std::string _database_name;
    std::string _login_name;
    std::string _passwd;
};
#endif
