#include "project.h"

/*
Inorder manner : First print (n-1) child then root then last child.
*/
void inorder(pid_t arrayPID[])
{
    for (size_t i = 0; i < count - 1; i++)
    {
        siginfo_t sig1;
        waitid(P_PID, arrayPID[i], &sig1, WSTOPPED);  // Waiting for the (n-1) children to execute their second SIGSTOP.

        kill(arrayPID[i], SIGCONT); // Sending SIGCONT to stopped (n-1) children in inorder fashion.

        siginfo_t sig;
        waitid(P_PID, arrayPID[i], &sig, WEXITED); // Waiting for (n-1) children to exit.
        int status = sig.si_status;
        printf("Exit status received from child no. : %ld whose PID : %d is : %d\n", i + 1,arrayPID[i], status);
    }

    printf(MAG "Internal node pid : %d and Parent PID : %d\n" RESET , getpid(), getppid()); // Print its own details.
    fflush(stdout);

    siginfo_t sig1;
    waitid(P_PID, arrayPID[count-1], &sig1, WSTOPPED); // Waiting for the last child to execute their second SIGSTOP.

    kill(arrayPID[count-1], SIGCONT);// Sending SIGCONT to the last child.

    siginfo_t sig;
    waitid(P_PID, arrayPID[count-1], &sig, WEXITED); // Waiting for the last child to exit.
    int status = sig.si_status;
    printf("Exit status received from child no. : %d whose PID : %d is : %d\n", count,arrayPID[count-1], status);
}