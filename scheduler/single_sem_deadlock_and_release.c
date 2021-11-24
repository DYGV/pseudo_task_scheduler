#include <stdio.h>
#include <stdlib.h>
/* タスクの登録と、スケジューラのエントリポイントにアクセスしたい */
#include <schedule.h>
/* セマフォAPIを使いたい */
#include <sem.h>

static sem_t mutex;

// プログラムカウンタの値に関わらず常にセマフォを獲得しようとするタスク
void TaskA() {
    static int pc = 0;
    int ret = 0;
    ret = sem_wait(&mutex);
    printf(ret == 1 ? "A could get a token.\n" : "A could not get a token.\n");
    pc++;
}

// PC値 0: セマフォを獲得しようとする
// PC値 1: セマフォを解放しようとする
// PC値 2: 解放後のタスク
void TaskB() {
    static int pc = 0;
    int ret = 0;
    if (pc == 0) {
        ret = sem_wait(&mutex);
        printf(ret == 1 ? "B could get a token.\n" : "B could not get a token.\n");
    } else if (pc == 1) {
        ret = sem_post(&mutex);
        printf("B released a token.\n");
    } else if (pc == 2) {
        printf("hello from B\n");
    }
    if (ret == 1) {
        pc++;
    }
}


int main(void) {
    // セマフォsemを1で初期化
    if (sem_init(&mutex, 1) == -1) {
        perror("sem_init");
        exit(1);
    }
    // 走らせたいタスク
    void(*task[])() = {TaskA, TaskB};
    // タスクを登録する
    for (size_t i = 0; i < sizeof(task) / sizeof(task[0]); i++) {
        register_as_task(task[i]);
    }
    // エントリポイント
    schedule();
    return 0;
}
