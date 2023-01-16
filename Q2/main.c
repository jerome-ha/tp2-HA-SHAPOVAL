#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

/* DEFINITIONS */
char *hostname, *filename;
struct addrinfo hints;
struct addrinfo *result, *p;
char ipstr[INET_ADDRSTRLEN];
void *addr;

int main(int argc, char *argv[]){
	
	/* SAUVEGARDE DES NOMS DU SERVEUR ET DU FICHIER */
	if (argc==3){ // 3 car on compte le nom du programme + les arguments (serveur et nom du fichier)
		hostname = argv[1]; // argv[0] étant donc le nom du programme (gettftp ou puttftp par exemple)
		filename = argv[2];
		printf("Le nom du serveur est : %s\n",hostname);
		printf("Le nom du fichier est : %s\n",filename);
	}
	else{
		printf("Nombre d'arguments incorrect...");
		return -1;
	}
	
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;			// on s'intéresse aux adresses IPv4
	hints.ai_socktype = SOCK_DGRAM; 	// Datagram socket  
	hints.ai_protocol = 0; 				// on travaille avec n'importe quel protocole
	
	int s = getaddrinfo(hostname, NULL, &hints, &result);
	
	if (s!=0){ // Gestion de l'erreur (getaddrinfo renvoie une valeur négative lorsqu'il y a une erreur)
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s)); // nous renvoie quelle est l'erreur associée à notre getaddrinfo
        exit(EXIT_FAILURE);
	}
	
	else{
		p = result;
		while (p != NULL) { // on parcourt tous les enregistrements que nous donne result
			printf("L'adresse du serveur %s est : \n",hostname);
			
			// Identification de l'adresse courante
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr; 
			addr = &(ipv4->sin_addr); // permet de trouver l'adresse Internet

			// Conversion de l'adresse IP en une chaîne de caractères
			inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr); // la fonction inet_ntop convertit l'adresse addr en string ipstr
			printf(" IPv4 : %s\n", ipstr);

			// Adresse suivante
			p = p->ai_next;
		}
	}
	
	// Libération de la mémoire occupée par les enregistrements
	freeaddrinfo(result);

	return 0;
}

