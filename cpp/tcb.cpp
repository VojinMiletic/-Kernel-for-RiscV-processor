//
// Created by vojo on 7/30/23.
//
#include "../h/tcb.hpp"
#include "../h/syscall_c.h"
#include "../h/riscv.h"

TCB* TCB::running = nullptr;
time_t TCB::timeSliceCounter = 0; // Za asinhronu promenu konteksta
TCB::UspavaneNiti* TCB::headUspavane = nullptr;


TCB* TCB::t_create(TCB **handle, TCB::fooBody body, void *arg, uint64 *stack) {
    TCB* newThr = (TCB*)MemAllocator::init().mem_alloc((sizeof(TCB) + sizeof(Header) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    newThr->body = body;
    if(!newThr->body) newThr->stack = nullptr;
    else newThr->stack = stack;

    newThr->context = {(uint64)&ThreadWrapper, stack != nullptr ? (uint64)&stack[DEFAULT_STACK_SIZE * 8] : 0};
    newThr->finished = false;
    newThr->arg = arg;
    newThr->blocked = false;
    newThr->headNitiKojeMeCekaju = nullptr;
    newThr->timeSlice = DEFAULT_TIME_SLICE; // Za asinhronu promenu konteksta

    *handle = newThr;
    if(body != nullptr) Scheduler::init().put(newThr);
    return newThr;
}


void TCB::dispatch() {

    TCB* old = running;
    if(!old->isFinished() && !old->blocked){Scheduler::init().put(old);}
    running = Scheduler::init().get();
    ContextSwitch(&old->context, &running->context);
}

// Ovim oslobodimo sve niti koje nas cekaju
void TCB::freeWaiters() {
    while(running->headNitiKojeMeCekaju != nullptr){
        NitiKojeMeCekaju* tmp = running->headNitiKojeMeCekaju;
        tmp->nit->blocked = false;
        Scheduler::init().put(tmp->nit);
        running->headNitiKojeMeCekaju = running->headNitiKojeMeCekaju->next;
        tmp->next = nullptr;
        MemAllocator::init().mem_free(tmp);
    }
}

void TCB::exit() {
    TCB::running->setFinished(true);
    TCB::freeWaiters(); // Oslobodimo sve niti koje nas cekaju
    thread_dispatch();
}

void TCB::ThreadWrapper() {

    Riscv::popSppSpie(); //  Zbog asinhrone promene konteksta
    running->body(running->arg);

    TCB::exit();
}

// Uradjeno po uzoru na 2. zadatak, kolokvijum avgust 2021.
// Samo što mi možemo imati proizvoljan broj niti koje nas cekaju pa o njima vodimo evidenciju
void TCB::join(TCB *handle) {

    if(!handle) return;
    if(handle == TCB::running || TCB::running->isFinished() || handle->isFinished() || handle->blocked) return;

    NitiKojeMeCekaju* elem = (NitiKojeMeCekaju*)MemAllocator::init().mem_alloc((sizeof(NitiKojeMeCekaju) + sizeof(Header)
                                                                                    + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    elem->nit = TCB::running;
    elem->next = handle->headNitiKojeMeCekaju;
    handle->headNitiKojeMeCekaju = elem;

    TCB::running->blocked = true;
    thread_dispatch();
}

// Ovo je uradjeno po uputstvu iz teksta projekta
void TCB::t_sleep(time_t vreme) {
    if(vreme <= 0) return;
    UspavaneNiti* novi = (UspavaneNiti*)MemAllocator::init().mem_alloc((sizeof(UspavaneNiti) + sizeof(Header) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    novi->nit = TCB::running;
    novi->relativnoVremeBudjenja = vreme;
    novi->next = nullptr;

    UspavaneNiti* prev = nullptr;
    UspavaneNiti* tmp = headUspavane;
    while(tmp && novi->relativnoVremeBudjenja >= tmp->relativnoVremeBudjenja){
        novi->relativnoVremeBudjenja -= (tmp->relativnoVremeBudjenja - 1);
        prev = tmp;
        tmp = tmp->next;
    }
    if(prev){
        prev->next = novi;
    }
    else headUspavane = novi;
    novi->next = tmp;
    if(tmp) tmp->relativnoVremeBudjenja -= novi->relativnoVremeBudjenja;

    TCB::running->blocked = true;
    thread_dispatch();
}

void TCB::azurirajUspavane() {
    if(!headUspavane) return;
    headUspavane->relativnoVremeBudjenja -= 1;
}

void TCB::probudi() {
    if(!headUspavane) return;
    while(headUspavane && headUspavane->relativnoVremeBudjenja == 0){
        UspavaneNiti* tmp = headUspavane;
        headUspavane = headUspavane->next;
        tmp->next = nullptr;
        tmp->nit->blocked = false;
        Scheduler::init().put(tmp->nit);
        MemAllocator::init().mem_free(tmp);
    }
}