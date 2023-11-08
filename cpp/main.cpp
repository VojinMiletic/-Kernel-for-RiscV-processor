#include "../h/syscall_c.h"
#include "../h/riscv.h"
#include "../h/konzola.hpp"
#include "../h/syscall_cpp.hpp"
#include "../test/printing.hpp"

void userMain();

class Periodicna : public PeriodicThread{
public:
    Periodicna(time_t t) : PeriodicThread(t){}
    void periodicActivation() override{
        putc('a');
        putc(' ');
    }
};

class Sat : public PeriodicThread {
private:
    int m = 0, s = 0;
public:
    Sat(int mm, int ss) : PeriodicThread(10), m(mm), s(ss) {/*Prazan konstruktor*/}
    void periodicActivation() override{
        putc('\r');
        if(m < 10) putc('0');
        printInt(m);
        putc(':');
        if(s < 10) putc('0');
        printInt(s);
        if(m == 1) {
            putc('\n');
            this->terminate();
        }
        s++;
        if(s == 60){
            s = 0;
            m++;
            if(m == 60){
                m = 0;
            }
        }
    }
};


int main(){

    __asm__ volatile("csrs sstatus, 0x02");
    uint64 reg = ((uint64)&trap) | (uint64)1;
    __asm__ volatile("csrw stvec, %0" : : "r"(reg));

    thread_t* handle = (thread_t*) mem_alloc(sizeof(thread_t));
    thread_create(handle, nullptr, nullptr);

    TCB::running = *handle;

    Con::init();
    changeRegime();

/*
    Sat* s = new Sat(0,0);
    s->start();
    s->join();
*/
    userMain();


    Con::stani();

    return 0;
}