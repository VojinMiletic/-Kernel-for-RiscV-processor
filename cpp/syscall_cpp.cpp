//
// Created by vojo on 8/3/23.
//
#include "../h/syscall_cpp.hpp"


void* operator new(size_t sz){
    return mem_alloc(sz);
}
void operator delete(void* adr){
    mem_free(adr);
}

Thread::Thread(void (*body)(void *), void *arg) {
    this->body = body;
    this->arg = arg;
    // U metodi start cemo napraviti nit, jer ne zelimo da se ona ovde uvaci u scheduler
}
Thread::~Thread() noexcept {
    delete myHandle;
}
void Thread::dispatch() {
    thread_dispatch();
}
void Thread::join() {
    thread_join(myHandle);
}
int Thread::start() {
    thread_create(&myHandle,this->body, this->arg);
    return 0;
}

int Thread::sleep(time_t vreme) {
    return time_sleep(vreme);
}
void Thread::threadWrap(void *arg) {
    Thread* nit = (Thread*)arg;
    nit->run();
}
Thread::Thread() {
    this->body = threadWrap;
    this->arg = this;
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}
int Semaphore::signal() {
    return sem_signal(myHandle);
}
int Semaphore::wait() {
    return sem_wait(myHandle);
}
Semaphore::~Semaphore() {
    sem_close(myHandle);
}

PeriodicThread::PeriodicThread(time_t per) : Thread(), period(per) {/* Prazno telo konstruktora */}

void PeriodicThread::terminate() {
    this->period = -1;
}
void PeriodicThread::wrapper(void *arg) {
    PeriodicThread* thr = (PeriodicThread*)arg;
    thr->periodicActivation();
}
void PeriodicThread::run(){
    while((int)this->period != -1){
        thread_t nit;
        thread_create(&nit,&wrapper, this);
        time_sleep(period);
        thread_join(nit);
    }
}
PeriodicThread::~PeriodicThread() noexcept {
    this->period = -1;
}
char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}