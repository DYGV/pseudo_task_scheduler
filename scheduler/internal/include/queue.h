/**
 * @file queue.h
 */
#pragma once

struct TCB;
struct Queue {
    struct TCB* head;
    struct TCB* tail;
    //! 空かどうかの確認などに使う
    size_t size;
};

void push_back(struct Queue* queue, struct TCB* node);
struct TCB* pop_front(struct Queue* queue);
