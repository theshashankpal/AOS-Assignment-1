//
// Created by shashank on 9/27/21.
//

#ifndef ASSIGNMENT_2_PROJECT_H
#define ASSIGNMENT_2_PROJECT_H


#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h> /* for semaphores */ 
#include <unistd.h> /* for fork() and execv */
#include <sys/mman.h> /*Prot_READ constants*/
#include <mqueue.h> /* "O_CREAT" O_Constants*/
#include <wait.h> /* SIGCONT , SIGSTOP*/

#define SHARED_MEMORY_NAME "/SYSSHAREDMEMORY"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

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
