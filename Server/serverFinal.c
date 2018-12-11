#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 4444
/*------------------------------------------------------Server side--------------------------------------------------------------------*/
int main()
{

	int sockfd, ret;
	struct sockaddr_in serverAddr;
	int newSocket;
	struct sockaddr_in newAddr;
	socklen_t addr_size;
	char buffer[1024];
	pid_t childpid;
	int receive=1;
	FILE *fptr1,*fptr2;
	char ch;
	char c;
	int words;
	int inp;
	char filename[20];
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0)
	{
		printf("Error in connection.\n");
		exit(1);
	}
	printf("Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		printf("Error in binding.\n");
		exit(1);
	}
	printf("Bind to port:%d\n", 4444);

	if(listen(sockfd, 10) == 0)
	{
		printf("Listening....\n");
	}
	else
	{
		printf("Error in Listening.\n");
		exit(1);
	}


	while(1)
	{
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0)
		{
				printf("Error in Accepting\n");
     			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
		if((childpid = fork()) == 0)
		{
			close(sockfd);
    
			while(1)
			{
         		recv(newSocket,buffer,1024,0);
         		if(strcmp(buffer, "exit") == 0)
				{
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
          			close(newSocket);
          
					break;
				}
         		else
				{
         			if(buffer[0]==0)
					{
						  printf("Save as:\n");
						  scanf("%s",filename);
						  printf("receiving file\n");
						  fptr1 = fopen(filename,"w");           
		     
	       				 if(fptr1 == NULL)
						 {
		    			 	printf("File %s Cannot be opened.\n", filename);
						 }
	     				 else
	     				 {
							 bzero(buffer, 1024); 
						     int fr_block_sz = 0;
						     while((fr_block_sz = recv(newSocket, buffer, 1024, 0)) > 0)
						     {
									int write_sz = fwrite(buffer, sizeof(char), fr_block_sz, fptr1);
									if(write_sz < fr_block_sz)
									{
										perror("File write failed.\n");
									}
									bzero(buffer, 1024);
									if (fr_block_sz == 0 || fr_block_sz != 1024) 
									{
										break;
									}
							}
							if(fr_block_sz < 0)
       						{
			
								fprintf(stderr, "recv() failed due to errno \n");
			
							}
							printf("File received from Client!\n");
							fclose(fptr1);
						}
					}
          
         			else if(buffer[0]==1)
					{
					  printf("Enter file to send to client \n");
					  scanf("%s",filename);
					  fptr1=fopen(filename,"r");
         
       				   if(fptr1 == NULL)
					   {
							printf("ERROR: File %s not found.\n", filename);
							exit(1);
						}

						bzero(buffer, 1024); 
						int fs_block_sz; 
						while((fs_block_sz = fread(buffer, sizeof(char), 1024, fptr1)) > 0)
						{
							if(send(newSocket, buffer, fs_block_sz, 0) < 0)
							{
								fprintf(stderr, "ERROR: Failed to send file %s. \n", filename);
								break;
							}
							bzero(buffer,1024);
						}
						printf("File %s was sent to Client\n", filename);
         			}
        			else
					{
          				printf("Give valid input\n");
        			} 
         		} 
      		}
		}
	}
	close(newSocket);
	return 0;
}


