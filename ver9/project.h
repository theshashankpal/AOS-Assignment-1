//
// Created by shashank on 9/27/21.
//

#ifndef ASSIGNMENT_2_PROJECT_H
#define ASSIGNMENT_2_PROJECT_H


#include <stdio.h>
#include <stdlib.h>

#include <sys/syscall.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>
#include <stdint.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <fcntl.h>
#include <semaphore.h>
#include <limits.h>
#include <errno.h>


#define SHARED_MEMORY_NAME "/AVSSHSAEEEESD"
#define MESSAGE_QUEUE_NAME "/SGAERESASD"

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
mqd_t mqd;
sem_t *sem;


#endif //ASSIGNMENT_2_PROJECT_H
