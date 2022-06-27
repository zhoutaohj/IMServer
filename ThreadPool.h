#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <pthread.h>
#include <queue>
#include <vector>
#include "TaskQueue.h"
#include <memory>
#include "NoCopy.h"
#include "Message.h"
#include <memory>

class WorkThread;
struct TaskItem
{
	std::shared_ptr<Message> msg_sp;
	std::shared_ptr<Channel> channel_sp;
};

class ThreadPool :public NoCopy,public std::enable_shared_from_this<ThreadPool>
{
	public:
		ThreadPool();//default constructor
		ThreadPool(int size);
		~ThreadPool();
		void Init(int size);//initilize
		void Start();//start all thread
		void Stop();//stop all thread
		int  GetThreadSize() const;//get current thread numbers
		//void AddTask(std::shared_ptr<Message> );
		void AddTask(const TaskItem&);
	private:
		void ProcessTask();
	    friend void ThreadCleanUp(void*);
	    friend class WorkThread;
	private:
		//TaskQueue<std::shared_ptr<Message>> task_queue;
		TaskQueue<TaskItem> task_queue;
		int thread_pool_size;

		std::vector<std::unique_ptr< WorkThread>>thread_vec;
		bool run_flag;

};

void ThreadCleanUp(void *);
#endif
