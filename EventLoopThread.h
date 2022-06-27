#ifndef EVENT_LOOP_THREAD_H
#define EVENT_LOOP_THREAD_H
#include "Thread.h"
#include <memory>
#include <functional>
#include <poll.h>
#include <unordered_map>
#include <map>
#include <vector>

class Channel;
class ThreadPool;

class EventLoopThread:public Thread,public std::enable_shared_from_this<EventLoopThread>
{
public:
    EventLoopThread();
	EventLoopThread(const char* name,int fd,std::weak_ptr<ThreadPool> );
	~EventLoopThread();
	void Run();
    
private:
    int server_fd;
    int server_port;
    std::vector<pollfd> vec_poll;
    std::map<int,std::shared_ptr<Channel>> channel_map;
    std::weak_ptr<ThreadPool> thread_pool_wp;

};
#endif