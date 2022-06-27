#ifndef IMSERVER_H
#define IMSERVER_H
//#define INFTIM -1
#include <string>
#include <deque>
#include <map>
#include "Channel.h"
#include "TaskQueue.h"
#include "Message.h"


#define REGISTER_SERVER_PORT 10001
#define CHAT_SERVER_PORT 10002

class ThreadPool;
class EventLoopThread;
class IMServer 
{ 
public:
    IMServer(const std::string &,unsigned int );
    ~IMServer()=default;

    IMServer(const IMServer&) =delete;
    IMServer& operator=(const IMServer&) =delete;

    int start();

private:
    void process();
    void init();

private:
    std::string server_name;
    unsigned int server_port;
    std::shared_ptr<EventLoopThread> event_loop_thread; //reactor
    std::shared_ptr<ThreadPool> thread_pool_sp;//thread pool

};

#endif