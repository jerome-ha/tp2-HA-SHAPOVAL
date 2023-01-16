#include <stdio.h>

char *hostname, *filename;

int main(int argc, char *argv[]){
	
	if (argc==3){ 
		hostname = argv[1]; 
		filename = argv[2];
		printf("The server's name is : %s\n",hostname);
		printf("The file's name is : %s\n",filename);
	}
	else{
		printf("Nombre d'arguments incorrect...\n");
		return -1;
	}
	
	return 0;
}

