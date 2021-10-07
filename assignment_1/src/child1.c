#include "project.h"

struct shared_memory_structure *ptr;
int shm_fd;
int count = 0;
sem_t *sem;

int main(int argc, char *argv[])
{
    raise(SIGSTOP);

    // Connecting to shared memory segment.
    shm_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0660);
    if (shm_fd == -1)
    {
        perror("Child Shared Memory");
        return 1;
    }

    // Mapping shared segment to process's virtual space.
    ptr = (struct shared_memory_structure *)mmap(0, sizeof(struct shared_memory_structure), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("Mapping");
        return 1;
    }

    // Connecting to semaphore
    sem = &(ptr->semaphore);

    // Taking all the argument values.
    int even = atoi(argv[1]);
    int odd = atoi(argv[2]);
    int level = atoi(argv[3]);

    pid_t arrayPID[even > odd ? even : odd];

    if (level > 0)
    {
        // creating chidlren if PID even.
        if (getpid() % 2 == 0)
        {
            childCreation(even, level, arrayPID, argv);
        }

        // creating children if PID odd.
        else
        {
            childCreation(odd, level, arrayPID, argv);
        }
    }
    else
    {
        // Leaves will execute this part of the code.
        raise(SIGSTOP); // Sent the sigstop signal to self for the second time to get prepared for inorder.
        printf(GRN "Leaf PID : %d and Parent PID : %d\n" RESET, getpid(), getppid());
        fflush(stdout);
        exit(0);
    }

    raise(SIGSTOP); // Sent the sigstop signal to self for the second time to get prepared for inorder.

    inorder(arrayPID); // Started the inorder printing.

    // Unmapping the shared object from process's virtual space.
    munmap(ptr, sizeof(sizeof(struct shared_memory_structure)));

    // Closing the file descriptor of shared memory segment.
    close(shm_fd);

    exit(0);
}

// Will fork children and then immediately execv them.
void childCreation(int children, int level, pid_t arrayPID[], char *argv[])
{
    level = level - 1;
    char str[256];
    sprintf(str, "%d", level);
    int j = 0;

    // creating children
    while (j < children)
    {
        pid_t childPid = fork();
        if (childPid != 0)
        {
            arrayPID[count++] = childPid;

            // Critical Section started.
            sem_wait(sem);

            ptr->a = ptr->a + 1; // Incrementing to record chidlren of next level.

            sem_post(sem);
            // Critical Section ended.

            siginfo_t sig;
            waitid(P_PID, childPid, &sig, WSTOPPED); // waiting for the child to stop for the first time.

            j++;
        }

        else if (childPid == 0)
        {
            char *args[] = {"./child2", argv[1], argv[2], str, NULL};
            execv("./child2", args);
        }
        else
        {
            perror("Child Creation");
            exit(6);
        }
    }

    // Critical Section started.
    sem_wait(sem);

    ptr->b = ptr->b - 1; // using to synchronize betweem same level processes.

    if (ptr->b != 0)
    {
        sem_post(sem);
        // Critical Section ended for if.

        // Making all next level children busy wait here.
        while (ptr->child_1_mode != 1)
        {
            usleep(50);
        }
    }
    else
    {
        // Last process of a level to finish its child creation work will execute this part of the code.
        ptr->b = ptr->a; // making it ready for next level
        ptr->a = 0; 
        ptr->child_2_mode = 0; // changing modes.
        ptr->child_1_mode = 1; // changing modes.
        sem_post(sem);
        // Critical Section ended for else.
    }

    for (size_t i = 0; i < count; i++)
    {
        // Giving continue signal to its children.
        kill(arrayPID[i], SIGCONT);
    }
}