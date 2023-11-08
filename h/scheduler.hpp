//
// Created by vojo on 7/30/23.
//

#ifndef PROJECT_BASE_V1_1_5_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_5_SCHEDULER_HPP

class TCB;

struct Node{
    TCB* elem;
    struct Node* next;
};

class Scheduler{
private:
    static TCB* idle;
    static Node *head, *tail;
    static bool firstTime;
    static void idleFoo(void* arg);
public:
    static Scheduler& init();
    void put(TCB* nit);
    TCB* get();
};


#endif //PROJECT_BASE_V1_1_5_SCHEDULER_HPP
