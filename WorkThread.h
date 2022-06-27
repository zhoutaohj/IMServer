#ifndef WORK_THREAD_H
#define WORJ_THREAD_H
#include "Thread.h"
#include "ThreadPool.h"
#include <memory>
#include <functional>

class WorkThread:public Thread
{
	public:
		WorkThread(const char* name,std::weak_ptr<ThreadPool>);
		~WorkThread();
		void Run();
	private:
		std::weak_ptr<ThreadPool>  thread_pool_wp;

};
#endif
