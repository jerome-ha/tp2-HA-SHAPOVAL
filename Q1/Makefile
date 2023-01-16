all : prog

prog : main.o
	gcc main.o -o prog
	
main.o : main.c
	gcc -c main.c -g -Wall -o main.o
	
	
clean : 
	rm -f *.o
	rm -f prog
