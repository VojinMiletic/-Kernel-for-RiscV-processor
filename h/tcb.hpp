//
// Created by vojo on 7/30/23.
//


#ifndef PROJECT_BASE_V1_1_5_TCB_HPP
#define PROJECT_BASE_V1_1_5_TCB_HPP
#include "../h/MemAllocator.hpp"
#include "../h/scheduler.hpp"

class TCB{
public:
    ~TCB(){MemAllocator::init().mem_free(stack);}
    using fooBody = void (*)(void*);
    static TCB* t_create(TCB** handle, fooBody body, void* arg, uint64* stack);
    static TCB* running;
    void setFinished(bool val){finished = val;}
    bool isFinished() const {return finished;}
    static void t_sleep(time_t vreme);
private:

    struct Context{
        uint64 ra;
        uint64 sp;
    };

    fooBody body;
    void *arg;
    Context context;
    uint64* stack;
    bool finished;
    bool blocked;
    // Za asinhronu promenu konteksta
    time_t timeSlice;
    static time_t timeSliceCounter;

    struct NitiKojeMeCekaju{
        TCB* nit;
        struct NitiKojeMeCekaju* next;
    };

    NitiKojeMeCekaju* headNitiKojeMeCekaju;

    // Za time_sleep
    struct UspavaneNiti{
        TCB* nit;
        unsigned long relativnoVremeBudjenja;
        struct UspavaneNiti* next;
    };

    static UspavaneNiti* headUspavane;
    static void probudi();
    static void azurirajUspavane();

    friend class Sem;
    friend class Riscv;
    friend class Scheduler;

    static void ThreadWrapper();

    static void ContextSwitch(Context *oldContext, Context *newContext);

    static void dispatch();

    static void exit();

    static void freeWaiters();

    static void join(TCB* handle);


};


#endif //PROJECT_BASE_V1_1_5_TCB_HPP
