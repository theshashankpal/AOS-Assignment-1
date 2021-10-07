target : 
	gcc -o main main.c -lrt -pthread
	gcc -o parent parent.c -lrt -pthread
	gcc -o child1 child1.c inorder.c -lrt -pthread
	gcc -o child2 child2.c inorder.c -lrt -pthread