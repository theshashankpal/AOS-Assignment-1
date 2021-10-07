CC = gcc
OBJ = main.o parent.o child1.o child2.o inorder.o
CFLAGS = -c
LFLAGS = -lrt -pthread 

vpath %.c src
vpath %.h src

output: $(OBJ) 
	$(CC) main.o -o $@ $(LFLAGS)
	$(CC) parent.o -o parent $(LFLAGS)
	$(CC) child1.o inorder.o -o child1 $(LFLAGS)
	$(CC) child2.o inorder.o -o child2 $(LFLAGS)

main.o: main.c project.h
	$(CC) $(CFLAGS) $^

parent.o: parent.c project.h
	$(CC) $(CFLAGS) $^

child1.o: child1.c project.h
	$(CC) $(CFLAGS) $^

child2.o: child2.c project.h
	$(CC) $(CFLAGS) $^

inorder.o: inorder.c project.h
	$(CC) $(CFLAGS) $^

clean: 
	rm -rf *.o child1 child2 parent output