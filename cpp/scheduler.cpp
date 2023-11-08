//
// Created by vojo on 7/30/23.
//
#include "../h/scheduler.hpp"
#include "../h/MemAllocator.hpp"
#include "../h/syscall_c.h"
#include "../test/printing.hpp"
#include "../h/riscv.h"


Node* Scheduler::head = nullptr;
Node* Scheduler::tail = nullptr;
bool Scheduler::firstTime = true;
TCB* Scheduler::idle = nullptr;

void Scheduler::idleFoo(void *arg) {
    changeRegime(); // Da bi ovo bila korisnicka nit
    while(!Scheduler::head){
        // Cekamo da Scheduler ne bude prazan
    }

}

Scheduler& Scheduler::init() {

    static Scheduler single;
    return single;
}

void Scheduler::put(TCB *nit) {
    Node* node = (Node*)MemAllocator::init().mem_alloc((sizeof(Node) + sizeof(Header) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    node->elem = nit;
    node->next = nullptr;
    tail = (!head ? head : tail->next) = node;

}

TCB* Scheduler::get() {
    if(!head) {
        thread_create(&idle, idleFoo, nullptr);
    }
    Node* node = head;

    head = head->next;
    if(!head) tail = nullptr;

    TCB* nit = node->elem;
    MemAllocator::init().mem_free(node);
    return nit;
}