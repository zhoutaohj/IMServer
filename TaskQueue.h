#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <deque>
#include "Mutex.h"
#include "ConditionVar.h"

template <typename TaskType>
class TaskQueue
{  
public:
    TaskQueue();
    ~TaskQueue() =default;
    void push( const TaskType &);
    TaskType pop();

    size_t size();
private:
    std::deque<TaskType> _task_queue;
    Mutex _mutex;
    ConditionVar _condition_var;
    
};




#endif