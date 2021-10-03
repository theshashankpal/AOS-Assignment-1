#include "project.h"

int main(int argc, char *argv[])
{

    struct shared_memory_structure *ptr;

    // Connecting to shared memory segment.
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, 0660);
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

    int pid = getpid();

    pid_t arrayPID[even > odd ? even : odd];

    int count = 0;
    int j = 0;
    level = level - 1;
    char str[256];
    sprintf(str, "%d", level);
    if (pid % 2 == 0)
    {

        while (j < even)
        {
            pid_t childPid = fork();
            if (childPid != 0)
            {
                arrayPID[count++] = childPid;
                ptr->b = ptr->b + 1;
                int status = 0;
                siginfo_t sig;
                waitid(P_PID, childPid, &sig, WSTOPPED);
                j++;
            }
            else
            {
                char *args[] = {"./child1", argv[1], argv[2], str, NULL};
                execv("./child1", args);
            }
        }
    }
    else
    {
        while (j < odd)
        {
            pid_t childPid = fork();
            if (childPid != 0)
            {
                arrayPID[count++] = childPid;
                ptr->b = ptr->b + 1;
                int status = 0;
                siginfo_t sig;
                waitid(P_PID, childPid, &sig, WSTOPPED);
                j++;
            }
            else
            {
                char *args[] = {"./child1", argv[1], argv[2], str, NULL};
                execv("./child1", args);
            }
        }
    }

    for (size_t i = 0; i < count; i++)
    {
        kill(arrayPID[i], SIGCONT);
    }

    printf("~~~~~INORDER PRINTING GOING TO START~~~~~\n");

    for (size_t i = 0; i < count - 1; i++)
    {

        siginfo_t sig;
        waitid(P_PID, arrayPID[i], &sig, WEXITED);
        int status = sig.si_status;

        printf("PID : %d and Parent PID : %d\n", arrayPID[i], getpid());
        printf("Exit status received from child no. %ld : %d\n", i + 1, status);
    }

    printf("Root pid : %d and Parent PID :%d\n", getpid(), getppid());

    siginfo_t sig;
    waitid(P_PID, arrayPID[count - 1], &sig, WEXITED);
    int status = sig.si_status;
    printf("PID : %d and Parent PID : %d\n", arrayPID[count - 1], getpid());
    printf("Exit status received from child no. %d : %d\n", count, status);

    printf("~~~~~INORDER PRINTING ENDED~~~~~\n");

    while (wait(NULL) != -1)
        ;

    // Unmapping the shared object from process's virtual space.
    munmap(ptr, sizeof(sizeof(struct shared_memory_structure)));

    // Closing the file descriptor of shared memory segment.
    close(shm_fd);
}