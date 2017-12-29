# road-map
implementation of client server architecture (C++) for querying an road map stored in a data base system. The server is storing a the road map of a city (modeled as a graph) in disk pages simulated as files in my program. Clients would be querying this map through a simple get neighbors query. This query will take the following inputs: (a) source node, (b) number of neighbors (k). I have  wrote a BFS code which starts at this source node and returns the first k node ids found in the graph representation of the road map. Note that BFS may span across different pages. I have partition graph using two method (1) uniform partiton (2) BFS partition.


FOLDER CONTAINS FOLLOWING FILE.
1 server.cpp
2 project.cpp
3 client.cpp
4 slides

#######  FILE DISCRIPTION ##############
For Partitioning :
	from "project.cpp" uncomment main function from line number 727-731.

%%%%%%%%   Compile & Run: %%%%%%%%%%%
		g++ project.cpp
		./a.out			
	
	BFS_Partition()  tooks around 6-8 hours("Sorry and thanks for passion")
	Uniform_Partition()  tooks around 2-3 minutes("Happy")

Partition Done...............

We 

######## RUNNING SERVER & CLIENT ############## 
	server.cpp: contain server program,
	client.cpp: contain client program,

%%%%%%%	  Complie & Run  %%%%%%%%%%%%%

	g++ server.cpp -o server
	./server

	g++ client.cpp -o client_n  ; where n={1,2........,N} # of client want to create.
	./client_n


