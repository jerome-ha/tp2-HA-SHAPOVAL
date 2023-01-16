#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/* DEFINITIONS */
char *hostname, *filename;
struct addrinfo hints;
struct addrinfo *result;

int main(int argc, char *argv[]){
	
	if (argc==3){ // argc étant la longueur de la chaîne de caractères (donne le nombre d'arguments - 1)
		hostname = argv[1]; // argv[] est un tableau contenant notre commande principale (argv[0]) et ses arguments
		filename = argv[2];
		printf("Le nom du serveur est : %s\n",hostname);
		printf("Le nom du fichier est : %s\n",filename);
	}
	else{
		printf("Nombre d'arguments incorrect...");
		return -1;
	}
	
	s = getaddrinfo(NULL, argv[1], &hints, &result);
	printf(s);
	
	return 0;
}

