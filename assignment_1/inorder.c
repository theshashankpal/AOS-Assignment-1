#include <stdio.h>
#include <unistd.h>
#include <wait.h> /* SIGCONT , SIGSTOP*/

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void inorder(pid_t arrayPID[], int count, int level)
{
    for (size_t i = 0; i < count - 1; i++)
    {

        siginfo_t sig1;
        waitid(P_PID, arrayPID[i], &sig1, WSTOPPED);

        kill(arrayPID[i], SIGCONT);

        siginfo_t sig;
        waitid(P_PID, arrayPID[i], &sig, WEXITED);
        int status = sig.si_status;
        printf("Exit status received from child no. : %ld whose PID : %d is : %d\n", i + 1,arrayPID[i], status);
    }

    printf(MAG "Internal node pid : %d and Parent PID : %d\n" RESET , getpid(), getppid());
    fflush(stdout);

    siginfo_t sig1;
    waitid(P_PID, arrayPID[count-1], &sig1, WSTOPPED);

    kill(arrayPID[count-1], SIGCONT);

    siginfo_t sig;
    waitid(P_PID, arrayPID[count-1], &sig, WEXITED);
    int status = sig.si_status;
    printf("Exit status received from child no. : %d whose PID : %d is : %d\n", count,arrayPID[count-1], status);
}