#include "project.h"

struct shared_memory_structure *ptr;
int shm_fd;
int count = 0;

int main(int argc, char *argv[])
{

    // Connecting to shared memory segment.
    shm_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0660);
    if (shm_fd == -1)
    {
        perror("Parent Shared Memory");
        return 1;
    }

    // Mapping shared segment to process's virtual space.
    ptr = (struct shared_memory_structure *)mmap(0, sizeof(struct shared_memory_structure), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

    if (ptr == MAP_FAILED)
    {
        perror("Mapping");
        return 1;
    }

    // Taking all the argument values;
    int even = atoi(argv[1]);
    int odd = atoi(argv[2]);
    int level = atoi(argv[3]);

    pid_t arrayPID[even > odd ? even : odd];

    if (getpid() % 2 == 0)
    {
        childCreation(even, level, arrayPID, argv);
    }
    else
    {
        childCreation(odd, level, arrayPID, argv);
    }

    for (size_t i = 0; i < count; i++)
    {
        kill(arrayPID[i], SIGCONT);
    }

    printf(CYN "~~~~~INORDER PRINTING GOING TO START~~~~~\n" RESET);
    fflush(stdout);

    inorder(arrayPID);

    printf(CYN "~~~~~~~~~~INORDER PRINTING ENDED~~~~~~~~~~\n" RESET);
    fflush(stdout);

    // Unmapping the shared object from process's virtual space.
    munmap(ptr, sizeof(sizeof(struct shared_memory_structure)));

    // Closing the file descriptor of shared memory segment.
    close(shm_fd);
}

void childCreation(int children, int level, pid_t arrayPID[], char *argv[])
{
    int j = 0;
    level = level - 1;
    char str[256];
    sprintf(str, "%d", level);
    while (j < children)
    {
        pid_t childPID = fork();
        if (childPID != 0)
        {
            arrayPID[count++] = childPID;
            ptr->b = ptr->b + 1;
            siginfo_t sig;
            waitid(P_PID, childPID, &sig, WSTOPPED);
            j++;
        }
        else if (childPID == 0)
        {
            char *args[] = {"./child1", argv[1], argv[2], str, NULL};
            execv("./child1", args);
        }
        else
        {
            perror("Root Creation");
            exit (6);
        }
    }
}

void inorder(pid_t arrayPID[])
{
    for (size_t i = 0; i < count - 1; i++)
    {

        siginfo_t sig1;
        waitid(P_PID, arrayPID[i], &sig1, WSTOPPED);

        kill(arrayPID[i], SIGCONT);

        siginfo_t sig;
        waitid(P_PID, arrayPID[i], &sig, WEXITED);
        int status = sig.si_status;
        printf("Exit status received from child no. : %ld whose PID : %d is : %d\n", i + 1, arrayPID[i], status);
    }

    printf(RED "Root node pid : %d and Parent PID : %d\n" RESET, getpid(), getppid());
    fflush(stdout);

    siginfo_t sig1;
    waitid(P_PID, arrayPID[count - 1], &sig1, WSTOPPED);

    kill(arrayPID[count - 1], SIGCONT);

    siginfo_t sig;
    waitid(P_PID, arrayPID[count - 1], &sig, WEXITED);
    int status = sig.si_status;
    printf("Exit status received from child no. : %d whose PID : %d is : %d\n", count, arrayPID[count - 1], status);
}