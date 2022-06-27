#include "Thread.h"
#include <string.h>
#include <iostream>

Thread::Thread(const char* name):tid(0),thread_name(name)
{

}

Thread::~Thread()
{

}

void Thread::Start()
{
	int ret=0;
	ret=pthread_create(&tid,NULL,ThreadFun,(void*)this);
	if(ret!=0)
	{
		//LOG_INFO
	}
	else
		pthread_detach(tid);
}

void* Thread::ThreadFun(void *param)
{
	 Thread* pThread=(Thread *)param;
	 //std::cout<<pThread->thread_name<<" is Running"<<std::endl;
     pThread->Run();
	 return NULL;

}

void Thread::Run()
{
   return ;
}

void Thread::Stop()
{
	//pthread_cancel(tid);
}

void Thread::Join()
{
	if(tid!=0)
	   pthread_join(tid,NULL);
}

const std::string Thread::GetThreadName()
{
	return thread_name;
}

void Thread::SetThreadName(const std::string& name)
{
	thread_name=name;
}
