//
// Created by shashank on 9/27/21.
//

#ifndef ASSIGNMENT_2_PROJECT_H
#define ASSIGNMENT_2_PROJECT_H


#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define SHARED_MEMORY_NAME "/SYSSHAREDMEMORY"

struct message_buffer
{
    pid_t pid;
};

struct shared_memory_structure
{
    int a;
    int b;
    sem_t semaphore;
    int child_1_mode;
    int child_2_mode;

};

struct shared_memory_structure *ptr;
int shm_fd;
int count = 0;
sem_t *sem;


#endif //ASSIGNMENT_2_PROJECT_H
