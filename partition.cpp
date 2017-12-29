#include<bits/stdc++.h>
using namespace std;
#define SIZE 30000
#define LEVEL 100

struct coordinateof_file
{
	int x_coordinate;
	int y_coordinate;
};

struct qnode
{
	int latitude;
	int longitude;
	int id;
};

struct list_node
{
	int xmin;
	int xmax;
	int ymin;
	int ymax;
};

map<int,coordinateof_file> map_from_nodeId_to_latlong;

int xmin=INT_MAX,ymin=INT_MAX,xmax=INT_MIN,ymax=INT_MIN;

qnode output_vector[2000];
int out=0;

void maxmin_find()
{
	char v;
	int latitude,longitude,node_id;
	ifstream infile1;

	infile1.open("node.txt");
	while(infile1 >> v >> node_id >> latitude >> longitude)
	{
		if(xmin > latitude)
		{
			xmin = latitude;
		}
		if(ymin > longitude)
		{
			ymin = longitude;
		}
		if(xmax < latitude)
		{
			xmax = latitude;
		}
		if(ymax < longitude)
		{
			ymax = longitude;
		}
	}
	infile1.close();
}

void create_file_uniform()
{
	int i,j,k,l,rem;
	char v;
	int node_id,latitude,longitude;
	ostringstream appendstring;
	string filename;
	ofstream outfile;
	ifstream infile1;
	coordinateof_file obj;

	infile1.open("node.txt");

	system("mkdir -p temp_uniform_split_node");
	cout << "\n Folder is Created for Uniform_partition\n";
	cout << xmin << "   " << ymin;
	cout << endl;
	cout << xmax << "   " << ymax;
	cout << endl; 

	cout << "\nFile Creation Start\n";
	k=0;
	rem = xmin % SIZE;
	xmin -= rem;
	rem = xmax % SIZE;
	xmax += rem;
	rem = ymin % SIZE;
	ymin -= rem;
	rem = ymax % SIZE;
	ymax += rem;
	outfile.open("minmax.txt");
	outfile << xmin << "	" << ymin << "	" << xmax << "	" << ymax;
	outfile.close();

	for(i=xmin; i<= xmax; i+=SIZE,k++)
	{
		l=0;
		for(j=ymin; j <= ymax; j+=SIZE,l++)
		{
			appendstring.str("");
			appendstring << "./temp_uniform_split_node/" << k << "_"<< l << ".txt";
			filename = appendstring.str();
			outfile.open(filename.c_str());
			outfile.close();
		}
	}

	while(infile1 >> v >> node_id >> latitude >> longitude) 
	{
		k = abs(latitude-xmin)/SIZE;
		l = abs(longitude-ymin)/SIZE;
		appendstring.str("");
		appendstring << "./temp_uniform_split_node/" << k << "_"<< l << ".txt";
		filename = appendstring.str();
		
		outfile.open(filename.c_str(),ios_base::app);
		outfile << node_id << "	" << latitude << "	" << longitude << "	" << "\n";

		obj.x_coordinate=latitude;
		obj.y_coordinate=longitude;
		map_from_nodeId_to_latlong.insert(pair <int,coordinateof_file>(node_id,obj));
		outfile.close();
	}
	cout << "\n Splitting node.txt file done\n";
}


void edgeConnection()
{
	system("rm -r Uniform_partition");
	system("mkdir -p Uniform_partition");
	int weight,NodeId1,NodeId2,node,k,l;
	int latitude1,longitude1,latitude2,longitude2;
	char a;
	ostringstream appendstring;
	string filename;

	ifstream edgefile,nodefile;
	ofstream final_file;
	edgefile.open("edge.txt");
	
	while(!edgefile.eof())
	{
		edgefile >> a >> NodeId1 >> NodeId2 >> weight;
		latitude1 = map_from_nodeId_to_latlong.find(NodeId1)->second.x_coordinate;
		longitude1 = map_from_nodeId_to_latlong.find(NodeId1)->second.y_coordinate;

		latitude2 = map_from_nodeId_to_latlong.find(NodeId2)->second.x_coordinate;
		longitude2 = map_from_nodeId_to_latlong.find(NodeId2)->second.y_coordinate;

		k = abs(latitude1-xmin)/SIZE; //file no.
		l = abs(longitude1-ymin)/SIZE;

		appendstring.str("");
		appendstring << "./temp_uniform_split_node/" << k << "_"<< l << ".txt";
		filename = appendstring.str();
		
		nodefile.open(filename.c_str());

		while(!nodefile.eof())
		{
			nodefile  >> node >> latitude1 >> longitude1;
		
			if(node == NodeId1)
			{ 

				appendstring.str("");
				appendstring << "./Uniform_partition/" << k << "_"<< l << ".txt";
				filename = appendstring.str();
				final_file.open(filename.c_str(),ios::app);
				final_file << node << "	"<< latitude1 << "	" << longitude1 << "	" << NodeId2 << "	" << latitude2 << "	" << longitude2 << "	"  << "	"<< weight << "\n";
				final_file.close();
				break;
			}
		}
		nodefile.close();
	}
	edgefile.close();
	system("rm -r temp_uniform_split_node");
	cout << "\nPartition Done\n";

}

void sortfile()
{

	ostringstream appendstring,appendstring_final;
	string filename;
	int i,j,k=0,l;
	ifstream infile;

	for(i=xmin; i<=xmax; i+=SIZE,k++)
	{
		l=0;
		for(j=ymin; j <= ymax; j+=SIZE,l++)
		{
			appendstring.str("");
			appendstring << "sort -g -k2 ./Uniform_partition/" << k << "_"<< l << ".txt -o ./Uniform_partition/" << k << "_"<< l << ".txt";
			filename = appendstring.str();
			//infile.open(filename.c_str());
			system(filename.c_str());	
			
			
		
		}
	}
	cout << "\n\n\n\n\nSorting of Node Id inside all file is done\n";
	
}

void Uniform_partition()
{
	
	maxmin_find();
	create_file_uniform();
	edgeConnection();
	sortfile();
	
}

void hashing()
{
	ifstream infile1;
	infile1.open("node.txt");
	char v;
	int node_id,latitude,longitude;
	coordinateof_file obj;
	map_from_nodeId_to_latlong.clear();

	while(infile1 >> v >> node_id >> latitude >> longitude) 
	{
		obj.x_coordinate=latitude;
		obj.y_coordinate=longitude;
		map_from_nodeId_to_latlong.insert(pair <int,coordinateof_file>(node_id,obj));
	}
}

void sortfile_BFS(int fno)
{
	int NodeId1,NodeId2,latitude1,longitude1,latitude2,longitude2,weight;
	ostringstream appendstring;
	string filename;
	int i;
	ifstream infile;
	ofstream outfile;
	outfile.open("BFS_MIN_MAX.txt");
	for(i=0; i< fno; i++)
	{
		appendstring.str("");
		appendstring << "sort -g -k2 ./BFS_partition/" << i << ".txt -o ./BFS_partition/" << i <<  ".txt";
		filename = appendstring.str();
		system(filename.c_str());

		appendstring.str("");
		appendstring << "./BFS_partition/" << i << ".txt";
		filename = appendstring.str();
		infile.open(filename.c_str());
		int bfs_xmin=INT_MAX,bfs_ymin=INT_MAX,bfs_xmax=INT_MIN,bfs_ymax=INT_MIN;
		while(!infile.eof())
		{

			infile >> NodeId1 >> latitude1 >> longitude1 >> NodeId2 >> latitude2 >> longitude2 >> weight;
			if(bfs_xmin > latitude1)
			{
				bfs_xmin = latitude1;
			}
			if(bfs_xmin > latitude2)
			{
				bfs_xmin = latitude2;
			}
			if(bfs_xmax < latitude1)
			{
				bfs_xmax = latitude1;
			}
			if(bfs_xmax < latitude2)
			{
				bfs_xmax = latitude2;
			}

			if(bfs_ymin > longitude1)
			{
				bfs_ymin = longitude1;
			}
			if(bfs_ymin > longitude2)
			{
				bfs_ymin = longitude2;
			}
			if(bfs_ymax < longitude1)
			{
				bfs_ymax = longitude1;
			}
			if(bfs_ymax < longitude2)
			{
				bfs_ymax = longitude2;
			}
		}
		infile.close();
		outfile << bfs_xmin << "	" << bfs_ymin << "	" << bfs_xmax << "	" << bfs_ymax << "\n";	
	}
	cout << "\n\n\n\n\nSorting of Node Id inside all file is done\n";
	outfile.close();
	
}


void BFS_partition()
{
	system("rm -r BFS_partition");
	system("mkdir -p BFS_partition");
	int NodeId1,NodeId2,weight;
	char a;
	queue <int> Queue;
	ostringstream appendstring;
	string filename;
	int fno=0,count=0,poped_id;
	ifstream edgefile;
	ofstream outfile; 
	map<int,bool> visited;
	visited.clear();

	hashing();
	edgefile.open("edge.txt");

	edgefile >> a >> NodeId1 >> NodeId2 >> weight;
	visited.insert(pair<int,bool>(NodeId1,1));
	Queue.push(NodeId1);
	edgefile.close();
	//cout << "\n Push  " << NodeId1;
	count = 0;
	while(!Queue.empty())
	{
		poped_id = Queue.front();
	//	cout << "\nPoped ID " << poped_id; 
		Queue.pop();
		if(count % LEVEL == 0)
		{
			outfile.close();
			appendstring.str("");
			appendstring << "./BFS_partition/" << fno << ".txt";
			filename = appendstring.str();
			fno++;
			cout << filename << endl;
			outfile.open(filename.c_str(),ios_base::app);
		}
		
		edgefile.open("edge.txt");
		while(!edgefile.eof())
		{
			edgefile >> a >> NodeId1 >> NodeId2 >> weight;
			if(NodeId1 == poped_id)
			{
				if(!(visited.find(NodeId2)->second))
				{
					Queue.push(NodeId2);
					//cout << "\n Push " << NodeId2;	
					visited.insert(pair<int,bool>(NodeId2,1));
				}
				outfile << NodeId1 << "	" << (map_from_nodeId_to_latlong.find(NodeId1)->second.x_coordinate);
				outfile <<"	" << (map_from_nodeId_to_latlong.find(NodeId1)->second.y_coordinate);
				outfile << "	" << NodeId2 << "	" << (map_from_nodeId_to_latlong.find(NodeId2)->second.x_coordinate);
				outfile << "	" << (map_from_nodeId_to_latlong.find(NodeId2)->second.y_coordinate) << "	" << weight;
				outfile << "\n";

			}
		}
		edgefile.close();
		count++;
	}
	edgefile.close();
	outfile.close();
	sortfile_BFS(fno);
	
}


int BFS_Query(int source_lat,int source_long,int no_of_outputNode)
{
	list_node lnode;
	int qcapacity=0,qsize;
	ostringstream appendstring;
	string filename;
	map<int,bool> visited;
	visited.clear();	
	queue <qnode> Queue;
	qnode q;
	int bfs_page=0;
	bool found = 0;
	vector<list_node> Vec_LIST;
	int i,length,latitude1,longitude1,latitude2,longitude2,tempNodeId,node_id1,node_id2,weight;
	ifstream infile;

	infile.open("BFS_MIN_MAX.txt");
	bfs_page++;
	while(!infile.eof())
	{
		infile >> xmin >> ymin >> xmax >> ymax;
		lnode.xmin = xmin;
		lnode.xmax = xmax;
		lnode.ymin = ymin;
		lnode.ymax = ymax;	
		Vec_LIST.push_back(lnode);
	}
	infile.close();
	length = Vec_LIST.size();

	for(i=0; i < length-1; i++)
	{
		if(source_lat >= Vec_LIST[i].xmin && source_lat <= Vec_LIST[i].xmax && source_long >= Vec_LIST[i].ymin && source_long <= Vec_LIST[i].ymax)
		{
			appendstring.str("");
			appendstring << "./BFS_partition/" << i << ".txt";
			filename = appendstring.str();
			infile.open(filename.c_str());
			bfs_page++;
			while(!infile.eof())
			{
				infile >> node_id1 >> latitude1 >> longitude1 >> node_id2 >> latitude2 >> longitude2 >>  weight;
				//cout << "\n Source : " << source_lat << "  latitude1 " << latitude1;
				if(source_lat == latitude1 && source_long == longitude1)
				{
					cout << "\nSource Node found, It is availabe in file name : " << i << ".txt\n";
					found = 1;
					tempNodeId = node_id1;
					visited.insert(pair<int,bool>(node_id1,1));
					q.latitude = latitude2;
					q.longitude = longitude2;
					q.id = node_id2;
					visited.insert(pair<int,bool>(node_id2,1));
					Queue.push(q);
					//cout << "\n Push  " << q.id;
					break;
				}
			}
			if(!found)
			{
				infile.close();
			}	
		}
		if(found)
		{
			break;
		}
	}
	if(!found)
	{
		cout << "\n Invalid Source ie source does not exist\n";
		cout << "Please Enter Correct source latitude and longitude\n";
		return 0;
	}

	while(no_of_outputNode)
	{
		qsize = Queue.size();
		if(qsize >= no_of_outputNode)
		{
			qcapacity = 1;
			break;
		}
		if(!infile.eof())
		{
			infile >> node_id1 >> latitude1 >> longitude1 >> node_id2 >> latitude2 >> longitude2 >> weight;
			//cout << "\n Read above " << node_id1;
		}
		else
		{
			tempNodeId = INT_MAX;
		}
		if(node_id1 == tempNodeId)
		{
			if(!(visited.find(node_id2)->second))
			{
				q.latitude = latitude2;
				q.longitude = longitude2;
				q.id = node_id2;
				Queue.push(q);
			//	cout << "\n Push  Above" << q.id;
				visited.insert(pair<int,bool>(node_id2,1));	
			}
			else
			{
				continue;
			}
			
		}
		else
		{
			q = Queue.front();
			//cout << "\n Node Id : " << (q.id) << "	Latitude : " << (q.latitude) << "	Longitude : " << (q.longitude);
			output_vector[out].id = q.id;
			output_vector[out].latitude = q.latitude;
			output_vector[out].longitude = q.longitude;
			out++;
			no_of_outputNode--;
			Queue.pop();
			//cout << "\n Pop  " << q.id;
			if(no_of_outputNode)
			{
				infile.close();
				found = 0;
				source_lat = (q.latitude);
				source_long = (q.longitude);
				//cout << "\n Pop latitude " << source_lat;

				for(i=0; i < length-1; i++)
				{
					if(source_lat >= Vec_LIST[i].xmin && source_lat <= Vec_LIST[i].xmax && source_long >= Vec_LIST[i].ymin && source_long <= Vec_LIST[i].ymax)
					{
						appendstring.str("");
						appendstring << "./BFS_partition/" << i << ".txt";
						filename = appendstring.str();
						infile.open(filename.c_str());
						bfs_page++;
						while(!infile.eof())
						{
							infile >> node_id1 >> latitude1 >> longitude1 >> node_id2 >> latitude2 >> longitude2 >>  weight;
							//cout << "\n filename " << filename;
							if(source_lat == latitude1 && source_long == longitude1)
							{
								found = 1;
								if(!(visited.find(node_id2)->second))
								{
										q.latitude = latitude2;
										q.longitude = longitude2;
										q.id = node_id2;
										Queue.push(q);
										//cout << "\n Push Below  " << q.id;
										visited.insert(pair<int,bool>(node_id2,1));
								}
								tempNodeId = node_id1;
								break;
							}
						}
						if(!found)
						{
							infile.close();	
						}
							
					}
					if(found)
					{
						break;
					}
				}

			} 
		}
	}
	infile.close();
	if(qcapacity=1)
	{
		while(no_of_outputNode--)
		{
			q = Queue.front();
			//cout << "\n Node Id : " << (q.id) << "	Latitude : " << (q.latitude) << "	Longitude : " << (q.longitude);
			output_vector[out].id = q.id;
			output_vector[out].latitude = q.latitude;
			output_vector[out].longitude = q.longitude;
			out++;
			Queue.pop();
		}
	}
	output_vector[out].id = bfs_page;
	out++;
	cout << "\n Query Done \n";
	return 1;
		
}
int Query(int source_lat,int source_long,int no_of_outputNode)
{
	int k,l,node_id1,latitude1,longitude1,node_id2,latitude2,longitude2,weight,qsize,check=0;
	int tempNodeId,qcapacity=0;
	ostringstream appendstring;
	string filename;
	map<int,bool> visited;
	visited.clear();	
	int uniform_page=0;
	ifstream infile;
	infile.open("minmax.txt");
	uniform_page++;
	infile >> xmin >> ymin >> xmax >> ymax;
	infile.close();
	queue <qnode> Queue;
	qnode q;
	ifstream query;
	if(source_lat < xmin || source_lat > xmax || source_long < ymin || source_long > ymax)
	{
		cout << "\n Invalid Source ie source does not exist\n";
		cout << "Please Enter Correct source latitude and longitude\n";
		return 0;
	}
	k = abs(source_lat - xmin)/SIZE;
	l = abs(source_long - ymin)/SIZE;
	cout << "Source : " << source_lat << "	" << source_long << endl;
	appendstring.str("");
	appendstring << "./Uniform_partition/" << k << "_"<< l << ".txt";
	filename = appendstring.str();
	query.open(filename.c_str());
	uniform_page++;

	cout << filename;
	while(!query.eof())
	{
		query >> node_id1 >> latitude1 >> longitude1 >> node_id2 >> latitude2 >> longitude2 >>  weight;
		if((int)latitude1 == source_lat && (int)longitude1 == source_long)
		{
				cout << "\nSource Node found, It is availabe in file name : " << k << "_"<< l << ".txt\n";
				tempNodeId = node_id1;
				check = 1;
				visited.insert(pair<int,bool>(node_id1,1));
				q.latitude = latitude2;
				q.longitude = longitude2;
				q.id = node_id2;
				visited.insert(pair<int,bool>(node_id2,1));
				Queue.push(q);
				//cout << "\n Push  " << q.id;
				break;	
		}
			
	}
	if(check==0)
	{
		cout << "\n Invalid Source ie source does not exist\n";
		cout << "Please Enter Correct source latitude and longitude\n";
		return 0;
	}
	while(no_of_outputNode)
	{
		qsize = Queue.size();
		if(qsize >= no_of_outputNode)
		{
			qcapacity = 1;
			break;
		}
		if(!query.eof())
		{
			query >> node_id1 >> latitude1 >> longitude1 >> node_id2 >> latitude2 >> longitude2 >> weight;
		}
		else
		{
			tempNodeId = INT_MAX;
		}
		if(tempNodeId == node_id1)
		{
			if(!(visited.find(node_id2)->second))
			{
				q.latitude = latitude2;
				q.longitude = longitude2;
				q.id = node_id2;
				Queue.push(q);
				//cout << "\n Push  " << q.id;
				visited.insert(pair<int,bool>(node_id2,1));	
			}
			else
			{
				continue;
			}
			
		}
		else
		{
			q = Queue.front();
			//cout << "\n Node Id : " << (q.id) << "	Latitude : " << (q.latitude) << "	Longitude : " << (q.longitude);
			output_vector[out].id = q.id;
			output_vector[out].latitude = q.latitude;
			output_vector[out].longitude = q.longitude;
			out++;
			
			no_of_outputNode--;
			Queue.pop();
			//cout << "\n Pop  " << q.id;
			if(no_of_outputNode)
			{
				query.close();
				k = abs((q.latitude)-xmin)/SIZE;
				l = abs((q.longitude)-ymin)/SIZE;
				appendstring.str("");
				appendstring << "./Uniform_partition/" << k << "_"<< l << ".txt";
				filename = appendstring.str();
				//cout << "\n File name " << filename;
				query.open(filename.c_str());
				uniform_page++;
				while(!query.eof())
				{
					query >> node_id1 >> latitude1 >> longitude1 >> node_id2 >> latitude2 >> longitude2 >> weight;
					if(node_id1 == q.id)
					{
						if(!(visited.find(node_id2)->second))
						{
							q.latitude = latitude2;
							q.longitude = longitude2;
							q.id = node_id2;
							Queue.push(q);
							//cout << "\n Push  " << q.id;
							visited.insert(pair<int,bool>(node_id2,1));
						}
						tempNodeId = node_id1;
						break;		
					}	
				}
			} 
		}
	}

	query.close();
	if(qcapacity=1)
	{
		while(no_of_outputNode--)
		{
			q = Queue.front();
			//cout << "\n Node Id : " << (q.id) << "	Latitude : " << (q.latitude) << "	Longitude : " << (q.longitude);
			output_vector[out].id = q.id;
			output_vector[out].latitude = q.latitude;
			output_vector[out].longitude = q.longitude;
			out++;
			Queue.pop();
		}
	}
	output_vector[out].id = uniform_page;
	out++;
	cout << "\n Query Done \n";
	return 1;
	
}
/*
int main()
{
	Uniform_partition();
	BFS_partition();	
}*/
