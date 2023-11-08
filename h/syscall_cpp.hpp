//
// Created by vojo on 8/3/23.
//

#ifndef PROJECT_BASE_V1_1_5_SYSCALL_CPP_HPP
#define PROJECT_BASE_V1_1_5_SYSCALL_CPP_HPP


#include "../h/syscall_c.h"
#include "syscall_c.h"


void* operator new (size_t sz);
void operator delete (void* adr);
class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();

    void join();
    static void dispatch ();
    static int sleep (time_t);

    int start ();

protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
    void (*body)(void*); void* arg;
    static void threadWrap(void* arg);
};
class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};
class PeriodicThread : public Thread { // Klasa je uradjena po uputstvu iz projekta
public:
    void terminate ();
    ~PeriodicThread() override;

protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
private:
    static void wrapper(void* arg);
    time_t period;
    void run() override;
};
class Console {
public:
    static char getc ();

    static void putc (char);
};


#endif //PROJECT_BASE_V1_1_5_SYSCALL_CPP_HPP
