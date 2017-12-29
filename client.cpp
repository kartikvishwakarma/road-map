#include<bits/stdc++.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>

using namespace std;
#define PORT 4443

struct qnode
{
	int latitude;
	int longitude;
	int id;
};

int Query_buffer[4]={0};
qnode output_vector[2000];

void QueryMenuDisplay()
{
	int source_lat,source_long,no_of_outputNode,type,check;
	
	char ch;

	
	cout << "\n-------------------Query Start --------------------------\n";
	cout << "\n Enter Co-ordinate of Source Latitude  :  ";
	cin >> source_lat;
	cout << "\n Enter Co-ordinate of Source Longitude  :  ";
	cin >> source_long;
	cout << "\nHow many number of Neighbours will you want to display  :  ";
	cin >> no_of_outputNode;
		
	do
	{
		check=0;

		cout << "\n-------------- Select anyone Partition Type -------------------- \n";
		cout << "\n 	1. UNIFORM PARTITION \n";
		cout << "\n 	2. BFS PARTITION \n";
		cout << "\n	3. EXIT FROM PROGRAM \n";
		cout << " => ";
		cin >> type;
		if(type == 1 || type == 2)
		{
			//Query(source_lat,source_long,no_of_outputNode);
			//type 1 for uniform and type 2 for BFS
			Query_buffer[0]=source_lat;
			Query_buffer[1]=source_long;
			Query_buffer[2]=no_of_outputNode;
			Query_buffer[3]=type;	
		}
		else if(type == 3)
		{
			exit(1);
		}
		else
		{	
			cout << "\n Invalid Entry, Try Again ! \n";
			check=1;
		}
	}
	while(check);
		
}



void connectionEstablished()
{
	struct sockaddr_in addr, cl_addr;  
	int sockfd, ret,i;  
	struct hostent * server;
	char * serverAddr;

	serverAddr = (char*)"127.0.0.1"; 

	sockfd = socket(AF_INET, SOCK_STREAM, 0);  
	if (sockfd < 0) 
	{  
		printf("Error creating socket!\n");  
		exit(1);  
	}  
	printf("Socket created...\n");   

	memset(&addr, 0, sizeof(addr));  
	addr.sin_family = AF_INET;  
	addr.sin_addr.s_addr = inet_addr(serverAddr);
	addr.sin_port = PORT;     

	ret = connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));  
	if (ret < 0) 
	{  
		printf("Error connecting to the server!\n");  
		exit(1);  
	}  
	printf("Connected to the server...\n");  
	
	
	
	int Query_buffer_size = (4*sizeof(int));
	while (true) 
	{
		QueryMenuDisplay();
		ret = sendto(sockfd, Query_buffer,Query_buffer_size, 0, (struct sockaddr *) &addr, sizeof(addr));  
		if (ret < 0) 
		{  
			printf("Error sending data!\n");  
		}
		//output_vector.clear();
		ret = recvfrom(sockfd,output_vector,(sizeof(qnode)*(Query_buffer[2]+1)), 0, NULL, NULL);  
		if (ret < 0) 
		{  
			printf("Error receiving data!\n");    
		}
		else if(ret==1) 
		{
			cout << "\n Invalid Source ie source does not exist";
			cout << "\n Please Enter Correct source latitude and longitude\n";
		}
		else 
		{
			printf("Received: ");
			
			for(i=0; i < Query_buffer[2]; i++)
			{
				cout << "\n Node Id : " << setw(10) << (output_vector[i].id) << "\tLatitude : " << (output_vector[i].latitude) << "\tLongitude : " << (output_vector[i].longitude);
			}
			cout << "\n\nTotal Data(Bytes) transferred from server to client is  : " << ret;
			cout << "\n Total Number of Page Access for the query is  " << (output_vector[i].id);
		}  

	}

}
  
int main() 
{  
	system("clear");
 	connectionEstablished();
	return 0;    
}  
