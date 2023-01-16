#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>

/* DEFINITIONS */
#define DATA_SIZE 512
#define IP_SIZE 512
#define RRQ_SIZE 128
#define ACK_SIZE 4

char *hostname, *filename, *sockaddr;
char ip_host[IP_SIZE];
struct addrinfo hints;
struct addrinfo *result, *p;
char ipstr[INET_ADDRSTRLEN]; // INET_ADDRSTRLEN est la longueur d'une adresse IPv4
void *addr;
int sock;
char rrq[RRQ_SIZE];
char dat[DATA_SIZE];
char ack[ACK_SIZE];

int main(int argc, char *argv[]){
	
	/* SAUVEGARDE DES NOMS DU SERVEUR ET DU FICHIER */
	if (argc==3){ // 3 car on compte le nom du programme + les arguments (serveur et nom du fichier)
		hostname = argv[1]; // argv[0] étant donc le nom du programme (gettftp ou puttftp par exemple)
		filename = argv[2];
		printf("Le nom du serveur est : %s\n",hostname);
		printf("Le nom du fichier est : %s\n",filename);
	}
	else if (argc==2){
		printf("Nom du fichier manquant, seul le nom du serveur a été sauvegardé\n");
		hostname = argv[1];
		printf("Le nom du serveur est : %s\n",hostname);
	}
	else{
		printf("Nombre d'arguments incorrect...");
		return -1;
	}
	
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;			// on s'intéresse aux adresses IPv4
	hints.ai_socktype = SOCK_DGRAM; 	// Datagram socket, utilisé pour les protocoles UDP 
	hints.ai_protocol = 0; 				// n'importe quel type de protocole
	
	int s = getaddrinfo(hostname, "1069", &hints, &result);
	
	/* Gestion des erreurs */
	if (s!=0){ // getaddrinfo renvoie une valeur négative lorsqu'il y a une erreur
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s)); // nous renvoie quelle est l'erreur associée à notre getaddrinfo
        exit(EXIT_FAILURE);
	}
		
	p = result;
	while (p != NULL) { // on parcourt tous les enregistrements que nous donne result
		printf("L'adresse du serveur '%s' est : \n",hostname);
		
		// Identification de l'adresse courante
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr; 
		addr = &(ipv4->sin_addr); // sin_addr permet de trouver l'adresse Internet

		// Conversion de l'adresse IP en une chaîne de caractères
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr); // la fonction inet_ntop convertit l'adresse addr en string ipstr
		printf("  IPv4 : %s\n", ipstr);

		// Adresse suivante
		p = p->ai_next;
	}
	
	/* Réservation d'un socket de connexion vers le serveur */
	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock ==-1){
		printf("Le socket n'a pas été créé \n");
		fprintf(stderr, "socket: %s\n", gai_strerror(sock)); // nous renvoie quelle est l'erreur associée à notre création de socket
		exit(EXIT_FAILURE);
	}
	
	/* a) 	Construction d'une requête en lecture (RRQ) */
	char *mode = "octet";
	
	// strcpy copie le 2ème 
	strcpy(&rrq[0],"0"); // RRQ correspond à opcode = 1 et WRQ correspond à opcode = 2, qu'on code sur 2 bits
	strcpy(&rrq[1],"1");
	strcpy(&rrq[2], filename);
	strcpy(&rrq[strlen(filename)+2],"0"); // notre  filename doit se terminer par un 0
	strcpy(&rrq[strlen(filename)+3],mode);
	strcpy(&rrq[strlen(filename)+ strlen(mode)+3],"0");
	printf("La requête envoyée est : %s\n",rrq);
	
	
	/* 		Envoi au serveur */
	int len_rrq = 2 + strlen(filename) + 1 + strlen(mode) + 1 + 1; // longueur de la requête (opcode : 2, filename+1, mode+1, +1)
	int send = sendto(sock, rrq, len_rrq, 0, result->ai_addr, result->ai_addrlen);
	if (send==-1){
		fprintf(stderr, "Le fichier n'a pas pu être délivré : %s\n",strerror(errno));
	}
	else{
		printf("Nombre d'octets renvoyés : %i\n",send);
	}
	
	
	// Libération de la mémoire occupée par les enregistrements
	freeaddrinfo(result);

	return 0;
}

