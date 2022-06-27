#include "DBConnectPool.h"
#include <algorithm>

DBConnectPool::DBConnectPool(const std::string& ip,const std::string& port,const std::string& name,const std::string& passwd)
{
    driver_ptr =nullptr;
    _db_address=ip;
    _db_port = port;
    _login_name = name;
    _passwd =passwd;

}

DBConnectPool::~DBConnectPool()
{
    
}

void DBConnectPool::InitPool(size_t con_count)
{
    driver_ptr =get_driver_instance();

    for (size_t i = 0; i != con_count; i++)
    {
        CreateConnnect();
    }
    
}

void DBConnectPool::CreateConnnect()
{
   //创建连接 "tcp://82.156.123.171:3306"
    char str[100];
    snprintf(str,sizeof(str),"tcp://%s:%s",_db_address.c_str(),_db_port.c_str());

    std::string str_connect =str;

	std::shared_ptr<sql::Connection>  con_ptr (driver_ptr->connect(str_connect, _login_name, _passwd));
    con_ptr->setSchema("imdatabase");

    MutexLock lock(_mutex);
    idle_connect_list.push_back(con_ptr); 
}

std::shared_ptr<sql::Connection> DBConnectPool::GetConnnect()
{
    MutexLock lock(_mutex);

    if(!idle_connect_list.empty())
    {
        std::shared_ptr<sql::Connection>  con_ptr = idle_connect_list.front();

        auto pos= idle_connect_list.begin();
        idle_connect_list.erase(pos);

        busy_connect_list.push_back(con_ptr);

        return con_ptr;
    }

    return nullptr;
}

void DBConnectPool::ReleaseConnnect(std::shared_ptr<sql::Connection> ptr)
{
    if (ptr!=nullptr)
    {
        MutexLock lock(_mutex);
        auto pos =std::find(busy_connect_list.begin(),busy_connect_list.end(),ptr);

        if(pos !=busy_connect_list.end() )
        {
            busy_connect_list.erase(pos);
            idle_connect_list.push_back(ptr);
        }
        
    }
    
}