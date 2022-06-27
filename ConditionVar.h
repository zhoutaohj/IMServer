#ifndef CONDITIONVAR_H
#define CONDITIONVAR_H
#include "NoCopy.h"
#include <pthread.h>

class Mutex;

class ConditionVar :public NoCopy
{
public:
    explicit ConditionVar(Mutex &);
    ~ConditionVar();

    void wait();
    void signal();
    void broadcast();
private:
    pthread_cond_t _condition_var;
    Mutex& _mutex;
};
#endif