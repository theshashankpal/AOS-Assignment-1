//
// Created by shashank on 9/27/21.
//

#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H


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


// Prints inorder
extern void inorder(pid_t[]);

// Creates children based on the value of arguments passed to it.
extern void childCreation(int, int, pid_t[], char *[]);

struct message_buffer
{
    pid_t pid;
};

struct shared_memory_structure
{
    int a; // Used to count next level children
    int b; // Used to keep track whether current level processes have finished their work or not
    sem_t semaphore;
    int child_1_mode; // Stops processes created by child1.c if its zero (Barrier)
    int child_2_mode; // Stops processes created by child2.c if its zero (Barrier)
};

extern struct shared_memory_structure *ptr;
extern int shm_fd;
extern int count ;
extern sem_t *sem;


#endif //ASSIGNMENT_H
