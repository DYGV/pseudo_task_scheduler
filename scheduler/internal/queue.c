/**
 * @file queue.c
 */

#include <stdio.h>
#include <queue.h>
#include <tcb.h>

/**
 * 構造体TCBをenqueueする関数
 * @param Queue * queue enqueueしたいキュー
 * @param struct* node 最後尾に入れたい構造体nodeアドレスへのポインタ
 * @return void
 */
void push_back(struct Queue* queue, struct TCB* node) {
    // 初めての要素のとき
    if (queue->head == NULL) {
        queue->head = node;
    } else {
        // 初めてでなければtailが存在するので、tailのnextにnodeを入れる
        queue->tail->next = node;
    }
    // tailの更新
    queue->tail = node;
    // queueサイズの更新
    queue->size++;
}

/**
 * 構造体nodeをdequeueする関数
 * @param struct Queue* queue 要素を取り出したいキュー
 * @return struct* head Queueの先頭にある構造体nodeのアドレスへのポインタ
 */
struct TCB* pop_front(struct Queue* queue) {
    // 要素がないならNULLを返す
    if (queue->size == 0) {
        return NULL;
    }
    struct TCB* head = queue->head;
    // queueの先頭の更新(headの次の要素)
    queue->head = queue->head->next;
    // 取り出したのでsizeをデクリメント
    queue->size--;
    return head;
}

