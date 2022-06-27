#include "DBManager.h"
#include <functional>
#include "Tool.h"


pthread_once_t DBManager::once_var = PTHREAD_ONCE_INIT;
std::shared_ptr<DBManager> DBManager::_dbmanager_sp =nullptr;

DBManager::DBManager(const std::string& address,const std::string& port,const std::string& name,const std::string& passwd):
_db_connnect_pool(address,port,name,passwd)
{
    int con_size=4;
    _db_connnect_pool.InitPool(con_size);
}

DBManager::~DBManager()
{

}

DBQueryResult DBManager::Query(const std::string & key,const std::string & sql)
{//convert to json fomart
    std::shared_ptr<sql::Connection> con = DBManager::GetDBManager()->_db_connnect_pool.GetConnnect();
    std::unique_ptr<sql::Statement> stat(con->createStatement());
    std::unique_ptr<sql::ResultSet> result(stat->executeQuery(sql));
    DBManager::GetDBManager()->_db_connnect_pool.ReleaseConnnect(con);

    return DBQueryResult(key,result.get());

}

int DBManager::Update(const std::string &sql)
{
    try
    {
        std::shared_ptr<sql::Connection> con = DBManager::GetDBManager()->_db_connnect_pool.GetConnnect();
        std::unique_ptr<sql::Statement> stat(con->createStatement());
        
        int ret = stat->executeUpdate(sql);
        DBManager::GetDBManager()->_db_connnect_pool.ReleaseConnnect(con);
        return ret;
    }
    catch (const sql::SQLException &e)
    {
        /*cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " »
             << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << » " )" << endl;*/
        return -1;
    }

    
}


void DBManager::Init()
{
    _dbmanager_sp.reset(new DBManager("127.0.0.1","3306","admin","12251225"));
}

std::shared_ptr<DBManager> DBManager::GetDBManager()
{
   pthread_once(&once_var,&DBManager::Init);
   return _dbmanager_sp;
    
}