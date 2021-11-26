#include <stdio.h>
#include <stdlib.h>
/* タスクの登録と、スケジューラのエントリポイントにアクセスしたい */
#include <schedule.h>
/* セマフォAPIを使いたい */
#include <sem.h>

static sem_t sem_1;
static sem_t sem_2;

void Task0() {
    static int pc = 0;
    int ret = 0;
    if (pc == 0) {
        ret = sem_wait(&sem_1);
        printf(ret == 1 ? "Task0 could get a token(sem_1).\n" : "Task0 could not get a token(sem_1).\n");
    } else if (pc == 1) {
        ret = sem_wait(&sem_2);
        printf(ret == 1 ? "Task0 could get a token(sem_2).\n" : "Task0 could not get a token(sem_2).\n");
    } else if(pc == 2) {
        printf("Hello from Task0.\n");
    }
    if (ret == 1) {
        pc++;
    }
}

void Task1() {
    static int pc = 0;
    int ret = 0;
    if (pc == 0) {
        ret = sem_wait(&sem_2);
        printf(ret == 1 ? "Task1 could get a token(sem_2).\n" : "Task1 could not get a token(sem_2).\n");
    } else if (pc == 1) {
        ret = sem_wait(&sem_1);
        printf(ret == 1 ? "Task1 could get a token(sem_1).\n" : "Task1 could not get a token(sem_1).\n");
    } else if(pc ==2) {
        printf("Hello from Task1.\n");
    }
    if (ret == 1) {
        pc++;
    }
}

int main(void) {
    // 1で初期化
    if (sem_init(&sem_1, 1) == -1 || sem_init(&sem_2, 1) == -1) {
        perror("sem_init");
        exit(1);
    }
    // 走らせたいタスク
    void(*task[])() = {Task0, Task1};
    // タスクを登録する
    for (size_t i = 0; i < sizeof(task) / sizeof(task[0]); i++) {
        register_as_task(task[i]);
    }
    // エントリポイント
    schedule();
    return 0;
}
