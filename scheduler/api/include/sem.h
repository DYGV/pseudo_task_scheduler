//! @file sem.h
#pragma once

typedef unsigned int sem_t;
int sem_init(sem_t* sem, unsigned int value);
int sem_post(sem_t* sem);
int sem_wait(sem_t* sem);
