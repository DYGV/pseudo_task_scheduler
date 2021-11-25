//! @file schedule.h

void register_as_task(void(* task)(void));
void schedule(void);
void __sem_init(unsigned int* sem);
void __wake_up(unsigned int* sem);
void __sleep_on(unsigned int* sem);
