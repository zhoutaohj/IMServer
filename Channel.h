#ifndef CHANNEL_H
#define CHANNEL_H
#include "NoCopy.h"
#include <vector>
#include <memory>
#include "EventLoopThread.h"
#include <atomic>
#include "Mutex.h"
/*
This class is used for the client and the server communication info management;
*/
class ThreadPool;
class Message;
class Channel: public NoCopy,public std::enable_shared_from_this<Channel>
{
public:
    Channel(int,std::weak_ptr<ThreadPool>,std::weak_ptr<EventLoopThread>);
    ~Channel();
    Channel(Channel &&) ;

public:
    int read();
    int write();
    size_t WriteBuffer(char* buff,size_t buff_size);
    size_t WriteMessagePackage(Message&);
    void analyze();
    short GetPollEvents();
    void SetPollEvents(short);
private:
    const int buff_size=1024;
    int socket_fd;
    std::vector<char> recv_buf;
    std::vector<char> send_buf;
    std::weak_ptr<ThreadPool> thread_pool_wp;
    std::weak_ptr<EventLoopThread> event_loop__wp;
    std::atomic_short  _poll_events;
    std::atomic_short _epoll_events;
    Mutex  _mutex;


};



#endif