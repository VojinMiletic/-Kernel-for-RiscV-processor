//
// Created by vojo on 7/28/23.
//


#ifndef PROJECT_BASE_V1_1_5_MEMALLOCATOR_HPP
#define PROJECT_BASE_V1_1_5_MEMALLOCATOR_HPP

#include "../lib/hw.h"
struct Header{
    size_t size; // Velicina u blokovima od po MEM_BLOCK_SIZE
    struct Header* next;
    struct Header* prev;
};

class MemAllocator{
private:
     static bool firstUse;
     static Header* FreeMemHead;
     void tryToMerge(Header* curr);
public:
     static MemAllocator& init();
     void* mem_alloc(size_t sz);
     int mem_free(void* adr);
};
#endif //PROJECT_BASE_V1_1_5_MEMALLOCATOR_HPP
