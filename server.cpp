#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include "project.cpp"

using namespace std;
#define PORT 4443
#define CLADDR_LEN 100


int Query_buffer[4]={0};



int main() 
{
 
	struct sockaddr_in addr, cl_addr;
	int sockfd, ret, newsockfd;
	socklen_t len;
	pid_t childpid;
	char clientAddr[CLADDR_LEN];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	{
		printf("Error creating socket!\n");
		exit(1);
	}
 	printf("Socket created...\n");
 
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = PORT;
 
	ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
	if (ret < 0) 
	{
		printf("Error binding!\n");
		exit(1);
	}
	printf("Binding done...\n");

	printf("Waiting for a connection...\n");
	listen(sockfd, 5);

	for (;;) 
	{ //infinite loop
		len = sizeof(cl_addr);
  		newsockfd = accept(sockfd, (struct sockaddr *) &cl_addr, &len);
  		if (newsockfd < 0) 
  		{
   			printf("Error accepting connection!\n");
   			exit(1);
  		}
  		printf("Connection accepted...\n");

		inet_ntop(AF_INET, &(cl_addr.sin_addr), clientAddr, CLADDR_LEN);
		if ((childpid = fork()) == 0) 
		{ //creating a child process
			close(sockfd); 


			for (;;) 
			{
				//output_vector.clear();

				ret = recvfrom(newsockfd,Query_buffer,(4*sizeof(int)), 0, (struct sockaddr *) &cl_addr, &len);
				if(ret < 0) 
				{
			 		printf("Error receiving data!\n");  
 					exit(1);
    			}
    			printf("Received data from %s: \n", clientAddr);
    			
    			cout << endl;
    			if(Query_buffer[3] == 1)
    			{
    				bzero(output_vector,(sizeof(qnode)*Query_buffer[2]));
    				out=0;
					Query(Query_buffer[0],Query_buffer[1],Query_buffer[2]);
    			}
    			else
    			{
    				bzero(output_vector,(sizeof(qnode)*Query_buffer[2]));
    				out=0;
    				BFS_Query(Query_buffer[0],Query_buffer[1],Query_buffer[2]);
    			}
    			if(out == 0)
    			{
    				ret = sendto(newsockfd,output_vector,1, 0, (struct sockaddr *) &cl_addr, len);
    			}
    			else
    			{
    				ret = sendto(newsockfd,output_vector,(sizeof(qnode)*(Query_buffer[2]+1)), 0, (struct sockaddr *) &cl_addr, len);
    			} 
    			   
    			if (ret < 0) 
    			{  
     				printf("Error sending data!\n");  
     				exit(1);  
    			}
    			
   			}
  		}
		close(newsockfd);
	}
}
