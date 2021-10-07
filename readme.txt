Name : Shashank Pal
-----------------------------------------------------------------------------------------------------------------------
List of Directories :

assignment_1 : It houses all the files related to this assignment.
assignment_1/src : It contains all the needed source and header files.
-----------------------------------------------------------------------------------------------------------------------
List of Files :

All source and header files are included in directory assignment_1/src :

main.c : Driver program which starts the whole execution by starting parent.c

parent.c : It's a code for root node. 
            I've seperated root code from all other nodes code, be it inner nodes or leaf nodes.

child1.c : I've used alternative level approach for barrier. So parent.c(root) starts child1.c .

child2.c : And child1.c starts child2.c, therafter child2.c starts child1.c and so on.. .
            So creation of children at a level keep on alternating between these files.
            Meaning child1.c creates level-1 children, child2.c creates level-2 chidlren ,
            child1.c creates level-3 children and so on.. 
            This keeps on repeating until leaf nodes are reached.

inorder.c : Supplemntary file to child1.c and child2.c , houses code for inorder printing.

project.h : Simple header file indicating what #includes are used,
             what functions and custom data structures that I'm using.
-----------------------------------------------------------------------------------------------------------------------
HOW TO COMPILE :

First run : make clean (to remove and unnecessary object files)
Then run : make
-----------------------------------------------------------------------------------------------------------------------
HOW TO RUN :

To run program, type following with required amount of arguments : ./output (even) (odd) (level)

For ex : ./output 2 3 3

Will create 2 children if pid is even , create 3 children if pid is odd . 
And there are total 4 levels starting from root level as : 0 (as per the example given by Sir)
-----------------------------------------------------------------------------------------------------------------------
Description Of The Structure Of My Program :

- First by running ./output (even) (odd) (level) we start main.c.

- Then main.c sets up the shared memory and semaphore that our program will use.
    And start parent.c which will be housing code for our root node.
    And will wait until parent.c finishes its work.
    (I've used named Shared Memory and anonymous Semaphore from POSIX interface.)

- Parent.c will link itself to the shared memory and semaphore.
    And will then store arguments from command line argument into some local variables.
    Depending on the level variable passed it will either call child1.c or not.
    After completing previous step will initiate inorder printing.
    After finishing inorder printing, it will remove the file descriptor of shared memory.
    And exit.

- Child1.c, will link itself to the shared memory and semaphore.
    It will either create children or not depending on the level passed to it.
    To create children, child2.c will be started (as we're following alternative level approach).
    Will stop when child1.c is started as a leaf node , or will wait for its children to finish.
    Then print pid and ppid.
    And then removes the file descriptor of shared memory.
    And exit.

- Child2.c, will link itself to the shared memory and semaphore.
    It will either create children or not depending on the level passed to it.
    To create children child1.c will be started (as we're following alternative level approach).
    Will stop when child2.c is started as a leaf node , or will wait for its children to finish.
    Then print pid and ppid.
    And then removes the file descriptor of shared memory.
    And exit.

- After everyone has exited and closed their file descriptors for shared memory ,
    main.c will destroy semaphore and unlink the shared memory.
    And exit.
-----------------------------------------------------------------------------------------------------------------------
BUGS :
None (To my limited knowledge)

If given more time , would've used pthread_barrier . As before I didn't know that we can use
pthread_barrier for processes too by passing non-zero arguments.
-----------------------------------------------------------------------------------------------------------------------