#include <stdio.h>
#include <stdlib.h>
/* タスクの登録と、スケジューラのエントリポイントにアクセスしたい */
#include <schedule.h>
/* セマフォAPIを使いたい */
#include <sem.h>

static sem_t mutex;

void Task0() {
    static int pc = 0;
    int ret = 0;
    if (pc == 0) {
        ret = sem_wait(&mutex);
        printf(ret == 1 ? "Task0 could get a token.\n" : "Task0 could not get a token.\n");
    } else if (pc == 1) {
        printf("Task0 tries to release a token.\n");
        ret = sem_post(&mutex);
    } else if (pc == 2) {
        printf("Hello from Task0\n");
    }
    if (ret == 1) {
        pc++;
    }
}

void Task1() {
    printf("Hello from Task1\n");
}

void Task2() {
    static int pc = 0;
    int ret = 0;
    if (pc == 0) {
        ret = sem_wait(&mutex);
        printf(ret == 1 ? "Task2 could get a token.\n" : "Task2 could not get a token.\n");
    } else if (pc == 1) {
        printf("Task2 tries to release a token.\n");
        ret = sem_post(&mutex);
    } else if (pc == 2) {
        printf("Hello from Task2\n");
    }
    if (ret == 1) {
        pc++;
    }
}

int main(void) {
    // 1で初期化
    if (sem_init(&mutex, 1) == -1) {
        perror("sem_init");
        exit(1);
    }
    // 走らせたいタスク
    void(*task[])() = {Task0, Task1, Task2};
    // タスクを登録する
    for (size_t i = 0; i < sizeof(task) / sizeof(task[0]); i++) {
        register_as_task(task[i]);
    }
    // エントリポイント
    schedule();
    return 0;
}
