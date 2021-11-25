/**
 * @file sem.c
 * @brief アプリケーションから使う排他制御のラッパールーチン
 * 細かいことはシステムタスクとしてOS内部で処理する。
 * @see schedule.c
 */
#include <sem.h>
#include <schedule.h>

/**
 * セマフォの初期化をする関数
 * @param sem_t *sem セマフォ\n
 * バイナリセマフォを想定しているので、\n
 * 値は原則として1だが、それより大きくても動作する
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
    // とりあえず眠っているタスクがあるか探してみる
    __wake_up(sem);
    return 1;
}

/**
 * sem上のトークンを取る関数
 * @param sem_t* sem トークンを取りたいsem
 * @return トークンが取れれば1、そうでなければ-1
 * @see __sleep_on(unsigned int* sem)
 */
int sem_wait(sem_t* sem) {
    if (*sem == 0) {
        __sleep_on(sem);
        return -1;
    }
    *sem -= 1;
    return 1;
}
