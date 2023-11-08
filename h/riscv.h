//
// Created by vojo on 7/29/23.
//

#ifndef PROJECT_BASE_V1_1_5_RISCV_H
#define PROJECT_BASE_V1_1_5_RISCV_H

#include "../lib/hw.h"
#include "buf.hpp"

extern "C" void supervisorTrap();
extern "C" void trap();

class Riscv{
public:

    static uint64 read_sstatus();
    static void write_sstatus(uint64 value);
    static void popSppSpie();

private:
    static void handleSyscall();
    static void handleTimer();
    static void handleConsole();

};

inline uint64 Riscv::read_sstatus() {
    uint64 value;
    __asm__ volatile("csrr %0, sstatus" : "=r"(value));
    return value;
}
inline void Riscv::write_sstatus(uint64 value) {
    __asm__ volatile("csrw sstatus, %0" : : "r"(value));
}


#endif //PROJECT_BASE_V1_1_5_RISCV_H
