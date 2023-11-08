//
// Created by vojo on 7/29/23.
//


#include "../h/riscv.h"
#include "../h/MemAllocator.hpp"
#include "../h/syscall_c.h"
#include "../lib/console.h"

#include "../test/printing.hpp"
#include "../h/konzola.hpp"


void Riscv::popSppSpie() {

    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSyscall() {
    uint64 kodOperacije;
    __asm__ volatile("mv %0, a0" : "=r"(kodOperacije));
    uint64 uzrok;
    __asm__ volatile("csrr %0, scause" : "=r"(uzrok));
    uint64 a3Vrednost; // Moramo ovo ovde jer kompajler registar a3 koristi za proveru if-ova
    __asm__ volatile("mv %0, a3" : "=r"(a3Vrednost));

    if(uzrok == 0x09 || uzrok == 0x08){
        uint64 volatile retPc; // adresa na koju cemo se vratiti( uvecana za 4 jer je to sirina jedne instrukcije)
        __asm__ volatile("csrr %0, sepc" : "=r"(retPc));
        retPc += 4; // Jer ecall u sepc upisuje adresu samog ecall a nama treba adresa instrukcije posle toga

        uint64 volatile status = read_sstatus(); // Status koji treba da kasnije restauriramo ( kap PSW na AR-u)

        //alokacija memorije
        if(kodOperacije == 0x01){
            uint64 argument;
            __asm__ volatile("mv %0, a1" : "=r"(argument));
            void* ret = MemAllocator::init().mem_alloc(argument);
            __asm__ volatile("mv a0, %0" : : "r"(ret));
        }
        // dealokacija
        else if(kodOperacije == 0x02){
            uint64 argument;
            __asm__ volatile("mv %0, a1" : "=r"(argument));
            void* arg = (void*)argument;
            int ret = MemAllocator::init().mem_free(arg);
            __asm__ volatile("mv a0, %0" : : "r"(ret));
        }

        //thread_create
        else if(kodOperacije == 0x11){
            uint64 rHandle;
            __asm__ volatile ("mv %0, a1" : "=r"(rHandle));
            TCB** handle = (TCB**)rHandle;

            uint64 rFoo;
            __asm__ volatile ("mv %0, a2" : "=r"(rFoo));
            TCB::fooBody Foo = (TCB::fooBody)rFoo;

            void* arg = (void*)a3Vrednost;

            uint64* stack = (uint64*)MemAllocator::init().mem_alloc((DEFAULT_STACK_SIZE * 8 + sizeof(Header) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);

            TCB::t_create(handle, Foo, arg, stack);

            uint64 ret = 0;
            if(!*handle) ret = -1;
            __asm__ volatile("mv a0, %0" : : "r"(ret));
        }
        // exit
        else if(kodOperacije == 0x12){
            TCB::exit();
        }
        // dispatch
        else if(kodOperacije == 0x13){
            TCB::dispatch();
        }
        // join
        else if(kodOperacije == 0x14){
            uint64 rHandle;
            __asm__ volatile("mv %0, a1" : "=r"(rHandle));
            TCB* handle = (TCB*)rHandle;
            TCB::join(handle);
        }
        // sem_open
        else if(kodOperacije == 0x21){
            uint64 rHandle;
            __asm__ volatile("mv %0, a1" : "=r"(rHandle));
            Sem** handle = (Sem**)rHandle;
            uint64 rInit;
            __asm__ volatile("mv %0, a2" : "=r"(rInit));
            unsigned init = (unsigned)rInit;
            int res = Sem::semOpen(handle, init);
            __asm__ volatile("mv a0, %0" : : "r"(res));
        }
        // sem_close
        else if(kodOperacije == 0x22){
            uint64 rHandle;
            __asm__ volatile("mv %0, a1" : "=r"(rHandle));
            Sem* handle = (Sem*)rHandle;

            int res = Sem::semClose(handle);
            __asm__ volatile("mv a0, %0" : : "r"(res));
        }
        // sem_wait
        else if(kodOperacije == 0x23){
            uint64 rHandle;
            __asm__ volatile("mv %0, a1" : "=r"(rHandle));
            Sem* handle = (Sem*)rHandle;

            int res = Sem::semWait(handle);
            __asm__ volatile("mv a0, %0" : : "r"(res));
        }
        // sem_signal
        else if(kodOperacije == 0x24){
            uint64 rHandle;
            __asm__ volatile("mv %0, a1" : "=r"(rHandle));
            Sem* handle = (Sem*)rHandle;

            int res = Sem::semSignal(handle);
            __asm__ volatile("mv a0, %0" : : "r"(res));
        }
        // Promena korisničkog režima
        else if(kodOperacije == 0x30){
            uint64 mask = 1 << 8;
            write_sstatus(status); // Vracamo status
            __asm__ volatile("csrc sstatus, %0" : : "r"(mask)); // Bit 8 sstatus registra odredjuje rezim(0-korisnicki, 1-sistemski)
            __asm__ volatile("csrw sepc, %0" : : "r"(retPc));
            __asm__ volatile("csrc sip, 0x02");
            return; // Posto smo promenili rezim skroz izlazimo iz ove funkcije
        }
        // time_sleep
        else if(kodOperacije == 0x31){
            uint64 rVreme;
            __asm__ volatile("mv %0, a1" : "=r"(rVreme));
            time_t vreme = (time_t)rVreme;
            TCB::t_sleep(vreme);
        }
        // getc
        else if(kodOperacije == 0x41){
            char c = Con::init().conGetc();
            __asm__ volatile("mv a0, %0" : : "r"(c));
        }
        //putc
        else if(kodOperacije == 0x42){
            uint64 rC;
            __asm__ volatile("mv %0, a1" : "=r"(rC));

            Con::init().conPutc((char)rC);
        }

        // Vracamo se iz sistemskog poziva poziva
        write_sstatus(status); // Vracamo status
        __asm__ volatile("csrw sepc, %0" : : "r"(retPc));
        __asm__ volatile("csrc sip, 0x02"); // U bit 1 SIP registra upisujemo 0, tako dajemo do znanja da smo obradili sistemski poziv
    }
    else{
       // Neidentifikovani prekid
    }
}

void Riscv::handleTimer() {
    // Obrada prekida tajmera
    __asm__ volatile("csrc sip, 0x02"); // Znak da smo obradili prekid

    // Azuriranje liste uspavanih niti
    TCB::azurirajUspavane();
    TCB::probudi();

    TCB::timeSliceCounter++;
    if(TCB::timeSliceCounter >= TCB::running->timeSlice){
        uint64 volatile sepcSaved;
        __asm__ volatile("csrr %0, sepc" : "=r"(sepcSaved));
        uint64 sstatusSaved = Riscv::read_sstatus();
        TCB::timeSliceCounter = 0;
        TCB::dispatch();
        Riscv::write_sstatus(sstatusSaved);
        __asm__ volatile("csrw sepc, %0" : : "r"(sepcSaved));
    }
}

void Riscv::handleConsole() {
    // prekid od konzole
   
    // Ovaj deo je urađen po uputstvu iz postavke projekta
    uint64 irq = plic_claim();
    if(irq == CONSOLE_IRQ) {
        while ((*(char *) CONSOLE_STATUS) & CONSOLE_RX_STATUS_BIT) {
            if(Con::init().getCountGetc() == Buf::KB)
                break;

            char c = *(char *) CONSOLE_RX_DATA;
            Con::init().putInGetc(c);
        }

    }
    plic_complete(irq);

}