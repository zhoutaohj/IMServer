#include "WorkThread.h"
#include <iostream>

WorkThread::WorkThread(const char* name,std::weak_ptr<ThreadPool> wp): Thread(name),thread_pool_wp(wp)
{

}


WorkThread::~WorkThread()
{

}

void WorkThread::Run()
{
	if(!thread_pool_wp.expired())
    {
		std::shared_ptr<ThreadPool> thread_pool_sp = thread_pool_wp.lock();
		thread_pool_sp->ProcessTask();
	} 
}
