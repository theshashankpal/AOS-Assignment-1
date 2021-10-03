#include "project.h"


int childCreation(int, int, pid_t[], char *[]);
void inorder(pid_t[], int, int);

int main(int argc, char *argv[])
{
    kill(getpid(), SIGSTOP);

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
        exit(0);
    }

    inorder(arrayPID, count, level);

    // Unmapping the shared object from process's virtual space.
    munmap(ptr, sizeof(sizeof(struct shared_memory_structure)));

    // Closing the file descriptor of shared memory segment.
    close(shm_fd);

    exit(0);
}

int childCreation(int children, int level, pid_t arrayPID[], char *argv[])
{

    level = level - 1;
    char str[256];
    sprintf(str, "%d", level);

    // creating children
    int j = 0;

    while (j < children)
    {

        pid_t childPid = fork();
        if (childPid != 0)
        {

            arrayPID[count++] = childPid;

            // Critical Section
            sem_wait(sem);

            ptr->a = ptr->a + 1;

            sem_post(sem);

            siginfo_t sig;
            waitid(P_PID, childPid, &sig, WSTOPPED);

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
            return 1;
        }
    }

    int check_first_time = 1;

    sem_wait(sem);

    ptr->b = ptr->b - 1;

    if (ptr->b != 0)
    {
        sem_post(sem);

        // Stopping all the next level children here.
        while (ptr->child_1_mode != 1)
        {
            usleep(500);
        }
    }
    else
    {
        ptr->b = ptr->a;
        ptr->a = 0;
        ptr->child_2_mode = 0;
        ptr->child_1_mode = 1;
        sem_post(sem);
    }

    for (size_t i = 0; i < count; i++)
    {
        kill(arrayPID[i], SIGCONT);
    }
    return level;
}

void inorder(pid_t arrayPID[], int count, int level)
{
    for (size_t i = 0; i < count - 1; i++)
    {

        siginfo_t sig;
        waitid(P_PID, arrayPID[i], &sig, WEXITED);
        int status = sig.si_status;

        printf("PID : %d and Parent PID : %d\n", arrayPID[i], getpid());
        printf("Exit status received from child no. %ld : %d\n", i + 1, status);
    }

    printf("Internal node pid : %d and Parent PID : %d\n", getpid(), getppid());

    siginfo_t sig;
    waitid(P_PID, arrayPID[count - 1], &sig, WEXITED);
    int status = sig.si_status;
    printf("PID : %d and Parent PID : %d\n", arrayPID[count - 1], getpid());
    printf("Exit status received from child no. %d : %d\n", count, status);
}
