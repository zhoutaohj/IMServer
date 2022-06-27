#include "ConditionVar.h"
#include "Mutex.h"

ConditionVar::ConditionVar(Mutex& mutex):
_mutex(mutex)
{  
    pthread_cond_init(&_condition_var,NULL);
}

ConditionVar::~ConditionVar()
{
    pthread_cond_destroy(&_condition_var);
}

void ConditionVar::wait()
{
    pthread_cond_wait(&_condition_var,&_mutex.mutex);
}

void ConditionVar::signal()
{
    pthread_cond_signal(&_condition_var);
}

void ConditionVar::broadcast()
{
    pthread_cond_broadcast(&_condition_var);
}
