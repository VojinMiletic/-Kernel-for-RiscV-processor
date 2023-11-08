//
// Created by vojo on 8/6/23.
//

#ifndef PROJECT_BASE_V1_1_5_BUF_HPP
#define PROJECT_BASE_V1_1_5_BUF_HPP

#include "../h/semaphore.hpp"
class Buf{
public:
    Buf();
    static const int KB = 256;
    char getChar();
    void putChar(char c);
    int getCount();
private:
    friend class Riscv;
    char buf[KB];
    Sem* itemAvailable;

    int head, tail , count;
};

#endif //PROJECT_BASE_V1_1_5_BUF_HPP
