#include "ThreadPool.h"
#include "WorkThread.h"
#include <stdio.h>

ThreadPool::ThreadPool()
{
	thread_pool_size=0;
	run_flag=true;
}

ThreadPool::ThreadPool(int size)
{
	thread_pool_size=0;
	run_flag=true;
	Init(size);
}

ThreadPool::~ThreadPool()
{

}

void ThreadPool::Init(int size)
{	
	thread_pool_size=size;
}

void ThreadPool::Start()
{
	char thread_name[255];
	for(int i=0;i!=thread_pool_size;i++)
	{
		snprintf(thread_name,sizeof(thread_name),"WorkThread NO.%d",i+1);
		thread_vec.push_back(std::make_unique<WorkThread>(thread_name, shared_from_this()));
	}

	for(int i=0;i!=thread_pool_size;i++)
	{
		thread_vec[i]->Start();	
	}
}

void ThreadPool::Stop()
{
	run_flag=false;
	for(int i=0;i!=thread_pool_size;i++)
	{
		thread_vec[i]->Stop();
	
	}

}

int ThreadPool::GetThreadSize() const
{
	return thread_pool_size;
}

/*void ThreadPool::AddTask(std::shared_ptr<Message> msg)
{
	task_queue.push(msg);
}*/

void ThreadPool::AddTask(const TaskItem& task)
{
	task_queue.push(task);
}


void ThreadPool::ProcessTask()
{
	pthread_cleanup_push(ThreadCleanUp,this);
	while(run_flag)
	{
		TaskItem  task = task_queue.pop();
		task.msg_sp->process(task.channel_sp);
	}
	pthread_cleanup_pop(1);

}

void ThreadCleanUp(void* param)
{
	//ThreadPool* pool=(ThreadPool*)param;
	//pthread_mutex_unlock(&pool->mutex_queue);
}
