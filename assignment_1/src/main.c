
#include "project.h"

struct shared_memory_structure *ptr;
int shm_fd;
sem_t *sem;

int main(int argc, char *argv[])
{
    // Unlinking shared memory if somehow previous runs were not successfull.
    shm_unlink(SHARED_MEMORY_NAME);
    
    // Checking if arguments entered is too few or too many.
    if (argc != 4)
    {
        printf("Error, no. of arguments you entered are too few or too many, please run it again : %d\n", argc);
        return 0;
    }

    // Taking all the argument values;
    int even = atoi(argv[1]);
    int odd = atoi(argv[2]);
    int level = atoi(argv[3]);

    // Checking if arguments passed are positive or not.
    if (even <= 0 || odd <= 0)
    {
        printf("Please enter non-zero positive arguments for no. of children that are needed to be made \n");
        exit (1);
    }

    if(level<0)
    {
        printf("Please enter non-negative argument for number of levels\n");
        exit(1);
    }

    // Creating shared memory segment
    shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR | O_EXCL, 0660);
    if (shm_fd == -1)
    {
        perror("Main Shared Memory");
        exit(4);
    }

    // Allocating space for our defined struct in shared memory.
    ftruncate(shm_fd, sizeof(struct shared_memory_structure));

    // Mapping it to this process's virtual space.
    ptr = (struct shared_memory_structure *)mmap(NULL, sizeof(struct shared_memory_structure), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("Main Mapping");
        exit(2);
    }

    // Initialization of a semaphore
    sem = &(ptr->semaphore);
    sem_init(sem, 1, 1);

    // starting our root program, which starts the sequence.
    pid_t programmePID = fork();

    if (programmePID == 0)
    {
        char *args[] = {"./parent", argv[1], argv[2], argv[3], NULL};
        execv("./parent", args);
    }
    else if (programmePID == -1)
    {
        perror("Error in creating parent program \n");
    }

    wait(NULL); // waiting for root to finish.

    fflush(stdout);

    // Unmapping the shared object from process's virtual space.
    munmap(ptr, sizeof(sizeof(struct shared_memory_structure)));

    // Closing the file descriptor of shared memory segment.
    close(shm_fd);

    // Unlinking shared memory segment , so it can be destroyed.
    shm_unlink(SHARED_MEMORY_NAME);

    // Destroying semaphore.
    sem_destroy(sem);
}