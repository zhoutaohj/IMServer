#include "TaskQueue.h"
#include "Message.h"
#include <memory>
#include "ThreadPool.h"
template <typename TaskType >
TaskQueue<TaskType>::TaskQueue()
:_mutex(),
_condition_var(_mutex)
{

}

template <typename TaskType>
void TaskQueue<TaskType>::push( const TaskType & task)
{
    MutexLock lock(_mutex);
    _task_queue.push_back(task);
    _condition_var.signal();
}

template <typename TaskType>
TaskType TaskQueue<TaskType>::pop()
{
    MutexLock lock(_mutex);

    while (_task_queue.empty())
    {
        _condition_var.wait();
    }

    TaskType task = *(_task_queue.begin());
    _task_queue.pop_front();

    return task;
     
}


template <typename TaskType>
size_t TaskQueue<TaskType>::size()
{
    MutexLock lock(_mutex);
    return _task_queue.size();
}



template class TaskQueue<TaskItem>;