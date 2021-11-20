/**
 * @file tcb.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <tcb.h>

/**
 * TCBの新しい要素を生成する関数
 * @param int id タスクID
 * @param void(*task)() run_task_on_RR_method()内で動かしたい戻り値がint型の関数ポインタ
 * @return 新しく確保したstruct TCBの先頭アドレスへのポインタ
 */
struct TCB* make_TCB_node(const int id, void(*task)(void)) {
    // 新しくstruct TCBのメモリ領域を確保
    struct TCB* tcb = (struct TCB*)malloc(sizeof(struct TCB));
    // エラー処理
    if (tcb == NULL) {
        perror("malloc()");
        exit(1);
    }
    // 新しく確保したstruct TCBのidにidを代入
    tcb->id = id;
    // 新しく確保したstruct TCBのtaskにtaskを代入
    tcb->task = task;
    tcb->next = NULL;
    // 先頭アドレスへのポインタを返す
    return tcb;
}

