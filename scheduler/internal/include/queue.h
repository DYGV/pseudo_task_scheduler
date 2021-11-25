//! @file queue.h
#pragma once

struct TCB; // opaque(tcb.hで定義)
struct Queue {
    struct TCB* head;
    struct TCB* tail;
    size_t size;
};

void push_back(struct Queue* queue, struct TCB* node);
struct TCB* pop_front(struct Queue* queue);
