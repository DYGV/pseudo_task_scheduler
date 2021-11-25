//! @file tcb.h
#pragma once

#define READY 1
#define RUN 2
#define WAIT 3

struct TCB {
    int id;
    int state;
    struct TCB* next;
    void(*task)();
};

struct TCB* make_TCB_node(const int id, void(*task)(void));
