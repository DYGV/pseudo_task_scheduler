//! @file schedule.c
#include <stdio.h>
#include <stdlib.h>
#include <tcb.h>
#include <queue.h>

static void __schedule(void);
static void idle(void);
static void add_ready_queue(struct TCB* node);
static struct TCB* pop_ready_queue(void);
static struct semaphore* search_semaphore(unsigned int* sem);

//! 内部処理用のセマフォ構造体
struct semaphore {
    //! セマフォのアドレスを入れておくため
    unsigned int* sem;
    //! そのsemにおけるwaitキュー
    struct Queue wait_queue;
    //! リストで管理する
    struct semaphore* next;
};

//! 現在動いているタスク
static struct TCB* running = {0};
//! readyキュー
static struct Queue ready_queue = {0};
//! 内部処理用のセマフォ変数
static struct semaphore* isem;
//! isemの先頭アドレスの保持用変数
static struct semaphore* isem_head;
//! 眠っているタスクの総数
static int sleeper = 0;

/**
 * 関数をもらい、構造体TCBを作ってenqueueする関数(タスクの登録時に使う)
 * @param void(*task)(void) タスクとして動かしたい関数ポインタ
 * @return void
 */
void register_as_task(void(* task)(void)) {
    // タスクのIDを割り振る(0, 1, 2, ...)
    static int task_id = 0;
    // TCBを作って、readyキューに追加する
    add_ready_queue(make_TCB_node(task_id++, task));
}

/**
 * スケジューラを呼ぶ関数
 * @param void
 * @return void
 */
void schedule(void) {
    // 終了できるように適当な回数で呼ぶ
    for (size_t i = 0; i < 12; i++) {
        __schedule();
    }
}

/**
 * TCBに登録した関数を走らせる関数
 * @param void
 * @return void
 */
static void __schedule(void) {
    // readyキューが空で動かせるタスクがないときはアイドルタスクを動かす
    // 今回はセマフォを解放するタスクをアイドルタスクとする
    while (((running = pop_ready_queue()) == NULL) && sleeper > 0) {
        printf("No task to run. Try to wakeup all task.\n");
        idle();
        printf("ok. Released all semaphore.\n");
    }
    running->task();
    printf("----------------\n");
    // タスクを抜けたときにRUN状態ならREADYキューへ、
    if (running->state == RUN) {
        add_ready_queue(running);
    }
}

/**
 * セマフォを初期化する関数
 * 内部用の構造体を作り、管理できるようにする
 * @param unsigned int* sem 新しく作りたいセマフォ
 * @return void
 */
void __sem_init(unsigned int* sem) {
    struct semaphore* new_sem;
    // セマフォごとにstruct semaphoreの領域を確保する
    if ((new_sem = (struct semaphore*)calloc(1, sizeof(struct semaphore))) == NULL) {
        perror("calloc");
        exit(1);
    }
    new_sem->sem = sem;
    new_sem->next = NULL;
    if (isem == NULL) {
        isem = new_sem;
        isem_head = new_sem;
        return;
    }
    isem->next = new_sem;
    // 変数をnextに更新する(先頭ポインタはグローバル変数で保持しているから大丈夫)
    isem = isem->next;
}

/**
 * struct semaphoreをsemで線形探索する関数
 * @param unsigned int* sem 探索したいセマフォのポインタ
 * @return struct semaphore* 検索結果
 */
static struct semaphore* search_semaphore(unsigned int* sem) {
    struct semaphore* concerned_sem = isem_head;
    while (concerned_sem != NULL && concerned_sem->sem != sem) {
        concerned_sem = concerned_sem->next;
    }
    return concerned_sem;
}

/**
 * 眠っているタスクを起こす関数
 * 引数semにおけるstruct semaphoreを探し、waitからreadyへ
 * @param unsigned int* sem タスクが眠っているセマフォ
 * @return void
 */
void __wake_up(unsigned int* sem) {
    if (sleeper == 0) {
        return;
    }
    // 該当のセマフォを探す
    struct semaphore* concerned_sem = search_semaphore(sem);
    // 該当のセマフォのウェイトキューからTCBを取り出す
    struct TCB* tcb = pop_front(&concerned_sem->wait_queue);
    if (tcb == NULL) {
        return;
    }

    // 今回はwaitからreadyに行くものとする
    add_ready_queue(tcb);
    // 起こしたのでsleeperを減らす
    sleeper--;
    printf("Task%dを状態%dへ遷移\n", tcb->id, tcb->state);
}

/**
 * タスクをsem上でスリープさせる関数
 * @param unsigned int * sem セマフォ
 * @return void
 */
void __sleep_on(unsigned int* sem) {
    // 該当のセマフォを探す
    struct semaphore* concerned_sem = search_semaphore(sem);
    // 状態を更新する
    running->state = WAIT;
    // 該当のセマフォのウェイトキューへTCBを入れる
    push_back(&concerned_sem->wait_queue, running);
    // 眠らせたのでsleeperを増やす
    sleeper++;
    printf("Task%dを状態%dへ遷移\n", running->id, running->state);
}

/**
 * アイドルタスク
 * 今回はセマフォを強制的に解放するタスク
 * @param void
 * @return void
 * @attention 動作するが、効率が悪い。idle()内でstruct semaphoreに関するループがあり、また__wake_up関数を呼ぶと探索があるので、走査が2回起きてしまう(改善の余地あり)。@sa __wake_up
 */
static void idle(void) {
    struct semaphore* concerned_sem = (struct semaphore*)isem_head;
    // struct semaphoreについてのループ
    while (concerned_sem != NULL) {
        // struct semaphoreのウェイトキューについてのループ
        while (concerned_sem->wait_queue.size != 0) {
            __wake_up(concerned_sem->sem);
        }
        (*concerned_sem->sem) = 1;
        concerned_sem = concerned_sem->next;
    }
}

/**
 * タスクをreadyキューに入れる関数
 * @param struct TCB* node readyキューに入れたいTCB
 * @return void
 */
static void add_ready_queue(struct TCB* node) {
    if (node == NULL) {
        return;
    }
    node->state = READY;
    push_back(&ready_queue, node);
}

/**
 * readyキューからタスクを取り出し、RUN状態にする関数
 * @param void
 * @ready struct TCB* readyキューの先頭
 */
static struct TCB* pop_ready_queue(void) {
    struct TCB* node;
    if ((node = pop_front(&ready_queue)) != NULL) {
        node->state = RUN;
    }
    return node;
}
