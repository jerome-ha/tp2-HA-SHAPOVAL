all : prog

prog : main.o
	gcc main.o -o prog
	
main.o : main.c fonctions.c
	gcc -c main.c fonctions.c -g -Wall -o main.o fonctions.o
	
	
clean : 
	rm -f *.o
	rm -f prog
