/**
 * @file sem.c
 * @brief アプリケーションから使う排他制御のラッパールーチン
 * 細かいことはタスクシステム側で処理する。
 * @see schedule.c
 */
#include <stdio.h>
#include <sem.h>
#include <schedule.h>

/**
 * セマフォの初期化をする関数
 * @param sem_t *sem セマフォ
 * @param unsigned int value セマフォの初期値
 * @return int 正常に処理されれば1
 * @see __sem_init(unsigned int* sem)
 **/
int sem_init(sem_t* sem, unsigned int value) {
    *sem = value;
    __sem_init(sem);
    return 1;
}

/**
 * sem上のトークンを手放す関数
 * @param sem_t* sem 手放すトークン
 * @return int 正常に処理されれば1
 * @see __wake_up(unsigned int* sem)
 */
int sem_post(sem_t* sem) {
    *sem += 1;
    if (*sem >= 1) {
        // 1以上ならとりあえず眠っているタスクがあるか探してみる
        __wake_up(sem);
    }
    return 1;
}

/**
 * sem上のトークンを取る関数
 * @param sem_t* sem トークンを取りたいsem
 * @return トークンが取れれば1、そうでなければ-1
 * @see __sleep_on(unsigned int* sem)
 */
int sem_wait(sem_t* sem) {
    if (*sem <= 0) {
        __sleep_on(sem);
        return -1;
    }
    *sem -= 1;
    return 1;
}
