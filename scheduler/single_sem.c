#include <stdio.h>
#include <stdlib.h>
/* タスクの登録と、スケジューラのエントリポイントにアクセスしたい */
#include <schedule.h>
/* セマフォAPIを使いたい */
#include <sem.h>

static sem_t mutex;

void TaskA() {
    static int pc = 0;
    int ret = 0;
    if (pc == 0) {
        ret = sem_wait(&mutex);
    } else if (pc == 1) {
       ret = sem_post(&mutex);
    } else if (pc == 2) {
        printf("hello from A\n");
    }
    if (ret == 1) {
        printf("A\n");
        pc++;
    }
}

void TaskB() {
    printf("B\n");
}

void TaskC() {
    static int pc = 0;
    int ret = 0;
    if (pc == 0) {
        ret = sem_wait(&mutex);
    } else if (pc == 1) {
        ret = sem_post(&mutex);
    } else if (pc == 2) {
        printf("hello from C\n");
    }
    if (ret == 1) {
        printf("C\n");
        pc++;
    }
}

int main(void) {
    // mutexを1で初期化
    if (sem_init(&mutex, 1) == -1) {
        perror("sem_init");
        exit(1);
    }
    // 走らせたいタスク
    void(*task[])() = {TaskA, TaskB, TaskC};
    // void(*task[])() = {TaskA, TaskC};
    // タスクを登録する
    for (size_t i = 0; i < sizeof(task) / sizeof(task[0]); i++) {
        register_as_task(task[i]);
    }
    // エントリポイント
    schedule();
    return 0;
}
