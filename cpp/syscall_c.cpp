//
// Created by vojo on 7/28/23.
//
#include "../h/syscall_c.h"
// U registru a0 cemo cuvati odgovarajuci kod operacije
// U registrima a1,a2,...itd. cemo cuvati argumente operacija, sleva udesno

void* mem_alloc(size_t sz){
    uint64 operation = 0x01; // Kod operacije za malloc
    size_t size = (sz + sizeof(Header) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    __asm__ volatile ("mv a1, %0" : : "r"(size));
    __asm__ volatile ("mv a0, %0" : : "r"(operation));
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return (void*)ret;
}

int mem_free(void* adr){
    uint64 operation = 0x02; // Kod operacije za free
    __asm__ volatile ("mv a1, %0" : : "r"(adr));
    __asm__ volatile ("mv a0, %0" : : "r"(operation));
    __asm__ volatile ("ecall");
    uint64 ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int thread_create(thread_t *handle, void(*start_routine)(void*), void* arg) {
    uint64 operation = 0x11; // Kod operacije za thread_create
    __asm__ volatile ("mv a3, %0" : : "r"(arg));
    __asm__ volatile ("mv a2, %0" : : "r"(start_routine));
    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("mv a0, %0" : : "r"(operation));

    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int thread_exit(){
    uint64 operation = 0x12; // Kod operacije za thread_exit()
    __asm__ volatile("mv a0, %0" : : "r"(operation));
    __asm__ volatile("ecall");
    return 0;
}

void thread_dispatch(){
    uint64 operation = 0x13; // Kod operacije za thread_dispatch()
    __asm__ volatile("mv a0, %0" : : "r"(operation));
    __asm__ volatile("ecall");
}

void thread_join(thread_t handle){
    uint64 operaton = 0x14; // Kod operacije za thread_join
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(operaton));
    __asm__ volatile("ecall");
}

int sem_open(sem_t* handle, unsigned init){
    uint64 operation = 0x21; // Kod operacije sa sem_open
    __asm__ volatile("mv a2, %0" : : "r"(init));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(operation));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_close(sem_t handle){
    uint64 operation = 0x22; // Kod operacije sa sem_close
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(operation));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_wait(sem_t handle){
    uint64 operation = 0x23; // Kod operacije sa sem_close
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(operation));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

int sem_signal(sem_t handle){
    uint64 operation = 0x24; // Kod operacije sa sem_close
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(operation));
    __asm__ volatile("ecall");
    int ret;
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

void changeRegime(){
    uint64 operation = 0x30; // Kod operacije za promenu korisničkog režima
    __asm__ volatile("mv a0, %0" : : "r"(operation));
    __asm__ volatile("ecall");
}

int time_sleep(time_t vreme){
    uint64 operation = 0x31; // Kod operacije za time_sleep
    __asm__ volatile("mv a1, %0" : : "r"(vreme));
    __asm__ volatile("mv a0, %0" : : "r"(operation));
    __asm__ volatile("ecall");
    return 0;
}

char getc(){

    uint64 operation = 0x41; // Kod operacije za getc()
    __asm__ volatile("mv a0, %0" : : "r"(operation));
    __asm__ volatile("ecall");
    char c;
    __asm__ volatile("mv %0, a0" : "=r"(c));
    return c;
}
void putc(char c){

    uint64 operation = 0x42; // Kod operacije za putc()
    __asm__ volatile("mv a1, %0" : : "r"(c));
    __asm__ volatile("mv a0, %0" : : "r"(operation));
    __asm__ volatile("ecall");

}
