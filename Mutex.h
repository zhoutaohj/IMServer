#ifndef MUTEX_H
#define MUTEX_H
#include "NoCopy.h"
#include <pthread.h>

class ConditionVar;
class Mutex : public NoCopy
{
 friend class MutexLock;
 friend class ConditionVar;
public:
    Mutex();
    ~Mutex();
private:
    void lock();
    void unlock();
private:
    pthread_mutex_t mutex;
};

class MutexLock :public NoCopy
{
public:
     explicit MutexLock( Mutex&);
    ~MutexLock();
private:
     Mutex& _mutex;
};

#endif