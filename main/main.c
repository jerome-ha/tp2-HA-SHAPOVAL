#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

#define port "1069"
#define DATA_SIZE 512
#define IP_SIZE 512
#define RRQ_SIZE 128
#define ACK_SIZE 4
#define BLOCK_SIZE 16
#define BUF_SIZE 512


char *hostname, *filename, *sockaddr;
char ip_host[IP_SIZE];
struct addrinfo hints;
struct addrinfo *result, *p;
char ipstr[INET_ADDRSTRLEN]; // INET_ADDRSTRLEN length IPv4 adress
void *addr;
int sock;
char rrq[RRQ_SIZE];
char dat[DATA_SIZE];
char ack[ACK_SIZE];
char buf[BUF_SIZE];

int main(int argc, char *argv[]){
	
	/* Q1 SAVE SERVER'S AND FILE'S NAME  */
	
	if (argc==3){ // 3: NAME OF THE PROGRAM, SERVER'S AND FILE'S NAME
		hostname = argv[1]; // argv[0] gettftp OR puttftp NAME OF PROGRAM
		filename = argv[2];
		printf("The server's name is : %s\n",hostname);
		printf("The file's name is : %s\n",filename);
	}
	else{
		printf("Incorrect argument...\n");
		return -1;
	}
	
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;		// IPv4	 Adresses
	hints.ai_socktype = SOCK_DGRAM; 	// Datagram socket, used for UDP protocols 
	hints.ai_protocol = IPPROTO_UDP; 	// any other protocol			
	
	int s = getaddrinfo(hostname, port, &hints, &result);
	
	/* ERROR */
	if (s!=0){ // getaddrinfo<0 IF ERROR
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s)); // What's the error getaddrinfo
        exit(EXIT_FAILURE);
	}
	
	else{
		p = result;
		while (p != NULL) { // We go through all the records that result gives us
			printf("Server %s 's adress is : \n",hostname);
			
			// Identification of the adress
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr; 
			addr = &(ipv4->sin_addr); // sin_addr can find the Internet adress
			// Conversion of the IP adress to a string
			inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr); // inet_ntop converts addr to string ipstr
			printf(" IPv4 : %s\n", ipstr);
			
			p = p->ai_next;// Next Adress
		}
	}
	/* Socket to the server */
	if ((sock= socket(result->ai_family, result->ai_socktype,result->ai_protocol))<0){
		perror("socket error");
		exit(EXIT_FAILURE);
	}

	/* a) Building a properly formed read request (RRQ) */
	char *mode = "byte";
	
	
	strcpy(&rrq[0],"0"); // RRQ -> opcode = 1 and WRQ -> opcode = 2, on 2 bits
	strcpy(&rrq[1],"1");
	strcpy(&rrq[2], filename);
	strcpy(&rrq[strlen(filename)+2],"0"); // Filename must end with a 0
	strcpy(&rrq[strlen(filename)+3],mode);
	strcpy(&rrq[strlen(filename)+ strlen(mode)+3],"0");
	printf("request is send : %s\n",rrq);
	
	/* Sending to the server */
	int len_rrq = 2 + strlen(filename) + 1 + strlen(mode) + 1 + 1; // length of request (opcode : 2, filename+1, mode+1, +1)
	int send = sendto(sock, rrq, len_rrq, 0, result->ai_addr, result->ai_addrlen);
	if (send==-1){
		fprintf(stderr, "the file couldn't be send : %s\n",strerror(errno));
	}
	else{
		printf("Number of byte sent : %i\n",send);
	}
	/* b) 	 Receiving a file consisting of a single data packet (DAT) and its acknowledgement (ACK) */	
	strcpy(&ack[0],"0"); // ack[0] and ack[1] represent opcode
	strcpy(&ack[1],"4");
	strcpy(&ack[2],&buf[2]); // ack[2] and ack[3] represent our block containing the packet data to acknowledge
	strcpy(&ack[3],&buf[3]);	
	
	ssize_t recv = recvfrom(sock, buf, BUF_SIZE, 0, result->ai_addr, &(result->ai_addrlen)); 
	printf("Number of byte received in buf : %li\n",recv);
	int nsend = sendto(sock, ack, sizeof(ack), 0, result->ai_addr, result->ai_addrlen);
	printf("Number of byte received : %i",nsend);

	// Freeing memory occupied by recordings
	freeaddrinfo(result);

	return 0;
}

