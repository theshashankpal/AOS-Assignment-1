

#include "project.h"

int main(int argc, char *argv[])
{
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

    if (even <= 0 || odd <= 0 || level <= 0)
    {
        printf("Please enter non-zero positive arguments \n");
        return 0;
    }

    // Creating shared memory segment
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR | O_EXCL, 0666);
    if (shm_fd == -1)
    {
        perror("Main Shared Memory");
        exit(4);
    }

    // allocating space for our defined struct in shared memory.
    ftruncate(shm_fd, sizeof(struct shared_memory_structure));

    // mapping it to this process's virtual space.
    ptr = (struct shared_memory_structure *)mmap(NULL, sizeof(struct shared_memory_structure), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("Mapping");
        return 0;
    }

    // Initialization of a semaphore
    sem = &ptr->semaphore;
    sem_init(sem, 1, 1);

    // starting our parent program, which starts the sequence.
    int pid = getpid();

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

    wait(NULL);

    // Unmapping the shared object from process's virtual space.
    munmap(ptr, sizeof(sizeof(struct shared_memory_structure)));

    // Closing the file descriptor of shared memory segment.
    close(shm_fd);

    // Unlinking shared memory segment , so it can be destroyed.
    shm_unlink(SHARED_MEMORY_NAME);

    // Destroying semaphore.
    sem_destroy(sem);
}