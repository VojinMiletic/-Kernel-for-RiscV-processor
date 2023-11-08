//
// Created by os on 8/6/23.
//
#include "../h/konzola.hpp"
#include "../h/riscv.h"
#include "../h/syscall_c.h"
bool Con::firstTime = true;
Buf* Con::getcBuf = nullptr;
Buf* Con::putcBuf = nullptr;
TCB* Con::putcNit = nullptr;

void Con::stani(){
    while (Con::init().getCountPutc() != 0 && (*((char *) CONSOLE_STATUS) & CONSOLE_TX_STATUS_BIT)) {
        char c = Con::init().removeFromPutc();
        *(char *) CONSOLE_TX_DATA = c;
    }
}

void Con::sacekaj(void* args){

    while(1) {
        changeRegime();

        while (Con::init().getCountPutc() != 0 && ((*((char *) CONSOLE_STATUS)) & CONSOLE_TX_STATUS_BIT)) {
            char c = Con::init().removeFromPutc();
            *(char *) CONSOLE_TX_DATA = c;
        }
        thread_dispatch();
    }
}

Con& Con::init() {
    if(firstTime){
        thread_create(&putcNit, sacekaj, nullptr);

        putcBuf = new Buf();
        getcBuf = new Buf();
        firstTime = false;
    }
    static Con single;
    return single;
}

char Con::conGetc() {
    return getcBuf->getChar();
}

void Con::conPutc(char c) {
    putcBuf->putChar(c);
}

void Con::putInGetc(char c) {
    getcBuf->putChar(c);
}
char Con::removeFromPutc() {
    return putcBuf->getChar();
}
int Con::getCountGetc() {
    return getcBuf->getCount();
}
int Con::getCountPutc() {
    return putcBuf->getCount();
}