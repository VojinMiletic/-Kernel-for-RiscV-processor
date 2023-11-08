//
// Created by vojo on 7/28/23.
//

#include "../h/MemAllocator.hpp"


Header* MemAllocator::FreeMemHead = nullptr;
bool MemAllocator::firstUse = true;
MemAllocator& MemAllocator::init(){
    if(firstUse){

        FreeMemHead = (Header*)((char*)HEAP_START_ADDR);
        FreeMemHead->next = nullptr;
        FreeMemHead->prev = nullptr;
        FreeMemHead->size = ( (uint64)( (char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR ) - sizeof(MemAllocator) - sizeof(Header)) / MEM_BLOCK_SIZE;
        firstUse = false;
    }
    static MemAllocator single;
    return single;
}


void* MemAllocator::mem_alloc(size_t sz) {

    Header* ret = nullptr;
    for(Header* tmp = FreeMemHead; tmp; tmp = tmp->next){
        if(tmp->size >= sz){ // Nasli smo odgovarajuce parce
            ret = tmp;
            // Ostatak je manji od velicine jednog bloka
            if(tmp->size - sz == 0){
                if(tmp->prev) tmp->prev->next = tmp->next;
                else FreeMemHead = tmp->next;
                if(tmp->next) tmp->next->prev = tmp->prev;

                tmp->size = sz;
                tmp->next = nullptr;
                tmp->prev = nullptr;
                break;
            }
            // Ostatak je veci od velicine jednog bloka
            size_t remainSize = tmp->size - sz;
            tmp->size = sz;
            Header* newBlock = (Header*) ((char*)tmp + tmp->size * MEM_BLOCK_SIZE);
            newBlock->size = remainSize;
            newBlock->prev = tmp->prev;
            newBlock->next = tmp->next;
            if(tmp->prev) tmp->prev->next = newBlock;
            else FreeMemHead = newBlock;
            if(tmp->next) tmp->next->prev = newBlock;
            tmp->next = nullptr;
            tmp->prev = nullptr;
            break;
        }
    }
    return (char*) ret + sizeof(Header);
}


void MemAllocator::tryToMerge(Header* curr) {
    if(!curr) return;
    if(curr->next && (char*)curr + curr->size*MEM_BLOCK_SIZE == (char*)curr->next){
        curr->size += curr->next->size;
        curr->next = curr->next->next;
        if(curr->next) curr->next->prev = curr;
    }
}


int MemAllocator::mem_free(void *adr) {

    if((char*)adr < (char*)HEAP_START_ADDR || (char*)adr >= HEAP_END_ADDR) return -1;
    Header* pred = nullptr;
    if(FreeMemHead && (char*)adr >= (char*)FreeMemHead){
        for(pred = FreeMemHead; pred->next && (char*)adr > (char*)pred->next; pred = pred->next);
    }
    Header* newBlock = (Header*) ((char*)adr - sizeof(Header)); // Ova linija je malo drugacija nego u zadatku
    newBlock->prev = pred;
    if(pred) newBlock->next = pred->next;
    else newBlock->next = FreeMemHead;
    if(newBlock->next) newBlock->next->prev = newBlock;
    if(newBlock->prev) newBlock->prev->next = newBlock;
    else FreeMemHead = newBlock;

    tryToMerge(newBlock);
    tryToMerge(pred);
    return 0;
}