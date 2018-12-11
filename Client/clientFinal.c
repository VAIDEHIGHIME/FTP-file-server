#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 4444
/*------------------------------------------------------client side--------------------------------------------------------------------*/
int main()
{

	int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[1024];
  	char filename[20];
  	char f_name[30],client_server_files[30][1024];
  	char ip,ch;
    FILE *f,*f2;
    
    int words = 0,i,n=0,j,t=0;
    char c;
    int inp;
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0)
	{
		printf("Error in connection.\n");
		exit(1);
	}
	printf("Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		printf("Error in connection.\n");
		exit(1);
	}
	printf("Connected to Server.\n");

	while(1)
	{
    	printf("Do u want to continue::Y/N ");
    	scanf("%c",&ip);
    	if(ip=='N')
    	{
     		buffer[0]='e';
     		buffer[1]='x';
     		buffer[2]='i';
     		buffer[3]='t';
     		buffer[4]='\0';
			
     		send(clientSocket,buffer,strlen(buffer),0);

     		printf("Server: \t%s\n", buffer);
      		close(clientSocket);
			printf("Disconnected from server.\n");
			exit(1);
    	}
    	else if(ip=='Y')
    	{ 
			printf("\nSend or Receive: 0/1 ");
     		scanf("%hhd",&buffer[0]);
     		send(clientSocket,buffer,1024,0);
     		if(buffer[0]==0)
			{
           		printf("\nEnter File Name to send :");
		        scanf("%s", filename);         
          		strcpy(f_name,filename);
           		f=fopen(filename,"r");
         
        		if(f == NULL)
		   		{
					printf("ERROR: File %s not found.\n", filename);
					exit(1);
		    	}

				bzero(buffer, 1024); 
				int fs_block_sz; 
				while((fs_block_sz = fread(buffer, sizeof(char), 1024, f)) > 0)
				{
		    		if(send(clientSocket, buffer, fs_block_sz, 0) < 0)
		    		{
		        		fprintf(stderr, "ERROR: Failed to send file %s. \n", filename);
		        		break;
		   			}
		    		bzero(buffer,1024);
				}
				printf("Ok File %s from Client was Sent!\n", filename);
    			strcpy(client_server_files[n],f_name);
    			n++;
         }
     	else if(buffer[0]==1)
		{
		    for(j=0;j<n;j++)
		    {
		      printf("%s\n",client_server_files[j]);
		    }
          	printf("Enter file name to receive file from server as..\n");
          	scanf("%s",filename);
          	printf("receiving file\n");
          	f = fopen(filename,"w");    
       		if(f == NULL)
			{
		    	printf("File %s Cannot be opened.\n", filename);
			}
	     	else
	     	{
		    	bzero(buffer, 1024); 
		   		int fr_block_sz = 0;
	    		while((fr_block_sz = recv(clientSocket, buffer, 1024, 0)) > 0)
	    		{
					int write_sz = fwrite(buffer, sizeof(char), fr_block_sz, f);
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
				printf("FIle received from server!\n");
				fclose(f);
			}
     	}  
		else
		{
			printf("Give Valid Input...\n");
		}
	}
    else
	{
      printf("Give Valid Input..\n");
    }
	}

	return 0;
}
