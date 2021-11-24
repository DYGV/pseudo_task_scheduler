#include <stdio.h>
#include <stdlib.h>
/* タスクの登録と、スケジューラのエントリポイントにアクセスしたい */
#include <schedule.h>
/* セマフォAPIを使いたい */
#include <sem.h>

static sem_t sem_1;
static sem_t sem_2;

void TaskA() {
    static int pc = 0;
    int ret = 0;
    if (pc == 0) {
        ret = sem_wait(&sem_1);
        printf(ret == 1 ? "A could get a token(sem_1)\n" : "A could not get a token(sem_1)\n");
    } else if (pc == 1) {
        ret = sem_wait(&sem_2);
        printf(ret == 1 ? "A could get a token(sem_2)\n" : "A could not get a token(sem_2)\n");
    } else if (pc == 2) {
        printf("A tries to release a token.(sem_2)\n");
        ret = sem_post(&sem_2);
    } else if (pc == 3) {
        printf("A tries to release a token.(sem_1)\n");
        ret = sem_post(&sem_1);
    }else{
        printf("hello from A\n");
    }
    if (ret == 1) {
        pc++;
    }
}

void TaskB() {
    static int pc = 0;
    int ret = 0;
    if (pc == 0) {
        ret = sem_wait(&sem_1);
        printf(ret == 1 ? "B could get a token(sem_1)\n" : "B could not get a token(sem_1)\n");
    } else if (pc == 1) {
        ret = sem_post(&sem_1);
        printf("B released token.(sem_1)\n");
    } else if (pc == 2) {
        printf("hello from B\n");
    }
    if (ret == 1) {
        pc++;
    }
}

int main(void) {
    // セマフォsemを1で初期化
    if (sem_init(&sem_1, 1) == -1 || sem_init(&sem_2, 1) == -1) {
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
