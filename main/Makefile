all : gettftp

gettftp : main.o
	gcc main.o -o gettftp
	
main.o : main.c
	gcc -c main.c -g -Wall -o main.o
	
	
clean : 
	rm -f *.o
	rm -f gettftp
