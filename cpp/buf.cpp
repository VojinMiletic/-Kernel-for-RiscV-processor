//
// Created by vojo on 8/6/23.
//
#include "../h/buf.hpp"

Buf::Buf() : head(0), tail(0), count(0) {
    Sem::semOpen(&itemAvailable, 0);
}
char Buf::getChar() {
    Sem::semWait(itemAvailable);
    char c = buf[head];
    head = (head + 1) % KB;
    count--;
    return c;
}
void Buf::putChar(char c) {
    if(count < KB){
        buf[tail] = c;
        tail = (tail + 1) % KB;
        count++;
        Sem::semSignal(itemAvailable);
    }
}

int Buf::getCount() {
    return count;
}