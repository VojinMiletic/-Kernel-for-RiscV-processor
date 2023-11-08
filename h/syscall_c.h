//
// Created by vojo on 7/28/23.
//
// Ovo je uradjeno po uputstvu iz projektnog zadatka

#ifndef PROJECT_BASE_V1_1_5_SYSCALL_C_H
#define PROJECT_BASE_V1_1_5_SYSCALL_C_H

#include "../lib/hw.h"
#include "../h/MemAllocator.hpp"
#include "../h/tcb.hpp"
#include "../h/semaphore.hpp"

void* mem_alloc(size_t sz); // Velicina u bajtovima
int mem_free(void* adr);

typedef TCB* thread_t;
int thread_create(thread_t *handle, void(*start_routine)(void*), void* arg); // OC = 0x11
int thread_exit(); // OC = 0x12
void thread_dispatch(); // OC = 0x13
void thread_join(thread_t handle); // OC = 0x14

typedef Sem* sem_t;
int sem_open(sem_t* handle, unsigned init); // OC = 0x21
int sem_close(sem_t handle); // OC = 0x22
int sem_wait(sem_t id); // OC = 0x23
int sem_signal(sem_t id); // OC = 0x24

void changeRegime(); // OC = 0x30     // Ovaj sistemski poziv sam dodao

int time_sleep(time_t vreme); // OC = 0x31

char getc();  // OC = 0x41
void putc(char c); // OC = 0x42

#endif //PROJECT_BASE_V1_1_5_SYSCALL_C_H
