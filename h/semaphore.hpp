//
// Created by vojo on 7/31/23.
//
// Klasa je uradjena po uzoru na semafore iz skolsog jezgra, iz profesorove knjige

#ifndef PROJECT_BASE_V1_1_5_SEMAPHORE_HPP
#define PROJECT_BASE_V1_1_5_SEMAPHORE_HPP

#include "../h/tcb.hpp"

// Red blokiranih niti je implementiran na isti način kao i scheduler
struct Blocked{
    TCB* nit;
    struct Blocked* next;
};

class Sem{
public:
    static int semWait(Sem* handle);
    static int semSignal(Sem* handle);
    static int semOpen(Sem** handle, unsigned value);
    static int semClose(Sem* handle);
private:
    friend class Riscv;
    int value;
    // Implementacija reda blokiranih ce biti ista kao implementacija Scheduler-a

    Blocked* headBlocked;
    Blocked* tailBlocked;

    void putInBlocked(TCB* nit);
    TCB* getFromBlocked();

};


#endif //PROJECT_BASE_V1_1_5_SEMAPHORE_HPP
