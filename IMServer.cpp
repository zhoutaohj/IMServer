#include "IMServer.h"
#include <iostream>
#include <sys/select.h>
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <vector>
#include "ThreadPool.h"
#include "EventLoopThread.h"

IMServer::IMServer(const std::string & name,unsigned int port ):
server_name(name),server_port(port)
{
    
    //thread_pool_sp =std::make_shared<ThreadPool>(thread_size);
    //event_loop_thread =std::make_shared<EventLoopThread>();
}

void IMServer::init()
{
    int thread_size =4;
    thread_pool_sp =std::make_shared<ThreadPool>(thread_size);
    event_loop_thread =std::make_shared<EventLoopThread>("IM EvevtLoop Thread",server_port,thread_pool_sp);
}


int IMServer::start()
{
    init();
    event_loop_thread->Start();
    thread_pool_sp->Start();
    
    return 1;
}