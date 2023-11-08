//
// Created by vojo on 7/31/23.
//

#include "../h/semaphore.hpp"
#include "../h/syscall_c.h"

int Sem::semOpen(Sem **handle, unsigned int value) {
    Sem* sem = (Sem*) MemAllocator::init().mem_alloc((sizeof(Sem) + sizeof(Header) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    sem->value = (int)value;
    sem->headBlocked = nullptr;
    sem->tailBlocked = nullptr;

    *handle = sem;
    int ret = 0;
    if(!(*handle)) ret = -1;
    return ret;
}

void Sem::putInBlocked(TCB *nit) {
    Blocked* elem = (Blocked*)MemAllocator::init().mem_alloc((sizeof(Blocked) + sizeof(Header) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    elem->nit = nit;
    elem->next = nullptr;
    tailBlocked = (!headBlocked ? headBlocked : tailBlocked->next) = elem;
}

TCB* Sem::getFromBlocked() {
    if(!headBlocked) return nullptr;
    TCB* ret = headBlocked->nit;
    Blocked* old = headBlocked;
    headBlocked = headBlocked->next;
    if(!headBlocked) tailBlocked = nullptr;
    MemAllocator::init().mem_free(old);
    return ret;
}

int Sem::semClose(Sem *handle) {
    if(!handle) return -1;
    while(handle->headBlocked){
        Blocked* tmp = handle->headBlocked;
        TCB* nit = tmp->nit;
        nit->blocked = false;
        Scheduler::init().put(nit);
        handle->headBlocked = handle->headBlocked->next;
        tmp->next = nullptr;
        MemAllocator::init().mem_free(tmp);
    }
    MemAllocator::init().mem_free(handle);
    handle = nullptr;
    return 0;
}

int Sem::semWait(Sem *handle) {
    if(!handle) return -1;

    if(--handle->value < 0){
        handle->putInBlocked(TCB::running);
        TCB::running->blocked = true;
        thread_dispatch();
    }
    return 0;
}

int Sem::semSignal(Sem *handle) {
    if(!handle) return -1;
    ++(handle->value);
    if(handle->value <= 0){
        TCB* nit = handle->getFromBlocked();
        nit->blocked = false;
        Scheduler::init().put(nit);
    }
    return 0;
}