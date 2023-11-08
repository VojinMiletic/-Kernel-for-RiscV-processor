//
// Created by vojo on 8/6/23.
//
// Klasa je urađena po uputstvu iz projekta, čista fasadna klasa koja apstrahuje konzolu

#ifndef PROJECT_BASE_V1_1_5_KONZOLA_HPP
#define PROJECT_BASE_V1_1_5_KONZOLA_HPP
#include "../h/buf.hpp"
#include "../h/tcb.hpp"
class Con{
public:

    static Con& init();
    char conGetc();
    void conPutc(char c);
    void putInGetc(char c);
    char removeFromPutc();
    int getCountGetc();
    int getCountPutc();
    static void stani();
private:
    static void sacekaj(void* args);
    friend class Riscv;

    static TCB* putcNit;

    static bool firstTime;
    static Buf* putcBuf; // U njega smestamo karaktere koje hocemo da ispisujemo
    static Buf* getcBuf; // U njega primamo karaktere sa konzole
};

#endif //PROJECT_BASE_V1_1_5_KONZOLA_HPP
