#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>
#include <string>
#include "NoCopy.h"
class Thread :public NoCopy
{
	public:
		Thread(const char* );
		~Thread();
		void Start();
		virtual void Run();
		void Stop();
		void Join();
	    const std::string GetThreadName();
		void SetThreadName(const std::string&);
	private:
		static void* ThreadFun(void *);
	private:
		pthread_t       tid;
		pthread_attr_t  thread_attr;
		std::string     thread_name;
};
#endif

