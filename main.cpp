#include <bits/stdc++.h>
using namespace std ;

struct nodeData 
{	
	
	int owner_id ;
	double value ;
	string owner_name ;
		
} ;

struct node {
	
	time_t timestamp ; 
	struct nodeData data ;
	string nodeDataHash ;
	int nodeNumber ;
	string nodeId, referenceNodeId , childReferenceNodeId , genesisReferenceNodeId ;
	string hashValue ;
	vector<int> childNodes ; 
	double child_value_sum ;
	bool isEncrypted ;
		
} ;

int nodeCounter = 0, totalChoices = 8 ;

struct node nodeArray[1000005] ;

void printStandard()
{
	cout<<endl ;
	cout<<"1. Add a Genesis Node"<<endl ;
	cout<<"2. Add set of Child Nodes of a particular Node"<<endl ;
	cout<<"3. Add Child node that originates from a particular Node"<<endl ;
	cout<<"4. Print Longest Chain of Genesis node"<<endl ;
	cout<<"5. Print Longest Chain of any node"<<endl ;
	cout<<"6. Merge Two Nodes"<<endl ;
	cout<<"7. Edit value of a node"<<endl ;
	cout<<"8. Exit"<<endl ;
	cout<<endl ;
}

struct node inputNode( string ref_id , string gen_ref_id  )
{
	struct node temp ; 
	struct nodeData tempData ;
	
	// kind of default 
	temp.timestamp = clock() ;
	temp.child_value_sum = 0.0 ;
	temp.isEncrypted = false ;
	
	cout<<endl ;
	cout<<"Please enter details about the node "<<endl ;
	cout<<"Enter Owner Name : " ; cin>>tempData.owner_name ;
	cout<<"Enter Owner Id : " ; cin>>tempData.owner_id ;
	cout<<"Enter Node Value : " ; cin>>tempData.value ;
	cout<<endl ;
	
	temp.data = tempData ;
	temp.nodeNumber = nodeCounter ; 
	temp.nodeId = to_string(nodeCounter) ;
	temp.referenceNodeId = ref_id ;
	temp.childReferenceNodeId = "" ;
	temp.genesisReferenceNodeId = gen_ref_id ;
	
	// Compute Hash Values Here
	// temp.nodeDataHash
	// temp.hashValue
	
	
	return temp ;
	
}

void printNode(int ind)
{
	cout<<"TimeStamp : "<<nodeArray[ind].timestamp<<endl ;
	cout<<"nodenumber : "<<nodeArray[ind].nodeNumber<<endl ;
	cout<<nodeArray[ind].nodeId<<" "<<nodeArray[ind].referenceNodeId<<" "<<nodeArray[ind].childReferenceNodeId<<" "<<nodeArray[ind].genesisReferenceNodeId<<endl ;
	cout<<"Sum :"<<nodeArray[ind].child_value_sum<<endl ;
	
	cout<<"Data : "<<nodeArray[ind].data.value<<endl ;
}

void debug()
{
	cout<<endl ;
	for (int i=0;i<nodeCounter;i++)
	{
		cout<<"Details about Node"<<i<<" : "<<endl ;
		printNode(i) ; 
	}
}

bool vis[100005] ; 
int dist[100005], farthest, far_dist ;

void dfs(int u)
{
	vis[u] = true ;
	for (auto it : nodeArray[u].childNodes)
	{
		if (!vis[it])
		{
			dist[it] = dist[u] + 1 ; 
			if (dist[it] > far_dist)
			{
				far_dist = dist[it] ; 
				farthest = it ; 
			}
			
			dfs(it) ; 
		}
	}
}

vector<int> longestChain(int node_number)
{
	
	memset(vis,false,sizeof(vis)) ; 
	dist[node_number] = 0 ;
	far_dist = 0 ; farthest = node_number ; 
	
	dfs(node_number) ; 
	
	int tempnode = farthest ; 
	vector<int> ans ; 
	
	while ( tempnode != node_number )
	{
		ans.push_back(tempnode) ; 
		tempnode = stoi( nodeArray[tempnode].referenceNodeId ) ; 
	}
	
	ans.push_back(node_number) ; 
	
	reverse( ans.begin() , ans.end() ) ;
	
	return ans ; 
	
}

int main()
{
	
	cout<<"------------------------------ Start -----------------------------"<<endl ;
	
	int choice ; 
	int genesis = -1 ; 
	
	while (1)
	{
		cout<<endl ;
		cout<<"**********************************************"<<endl ;
		printStandard() ;
		
		cout<<"Enter Your Choice : " ;
		cin>>choice ;
		cout<<endl ;
		
		if (choice < 1 or choice > totalChoices)
		{
			cout<<"Incorrect Choice! Please Enter a value between 1 and "<<totalChoices<<endl ;

		}else if (choice == totalChoices)
		{
			cout<<"Thank You for using our software"<<endl ; 
			break ;
		}else if ( choice == 1)
		{
			
			// Adding a genesis node 
			
			if (genesis != -1)
			{
				cout<<"Genesis Node Already Exists"<<endl ; 
				continue ;
			}
			
			struct node newnode = inputNode( "" , to_string(nodeCounter) ) ;
			
			nodeArray[nodeCounter] = newnode ; 
			genesis = nodeCounter ; 
			nodeCounter++ ;
			
			cout<<"Genesis node created Successfully with node number = "<<(nodeCounter-1)<<endl ;
			
			debug() ; 
			
		}else if (choice == 2)
		{
			
			// adding child nodes of a particular node
			
			int node_number, child_nodes ; 
			
			cout<<"Enter Node Number of the node whose child nodes you want to create : " ; cin>>node_number ; 
			
			if (node_number >= nodeCounter)
			{
				cout<<"No Node exists with nodeNumber = "<<node_number<<endl ;
				continue ;
			}
			
			cout<<"How many child Nodes do you want to create ? : " ; cin>>child_nodes ; 
			
			vector<struct node> nodes ; 
			double tempSum = 0.0 ; 
			
			for (int i=1 ; i<= child_nodes ; i++)
			{
				cout<<"------------"<<endl ;
				cout<<"Details about Child Node "<<i<<" : "<<endl ;
				struct node newnode = inputNode( nodeArray[node_number].nodeId , nodeArray[node_number].genesisReferenceNodeId ) ;
				nodes.push_back(newnode) ; 
				
				tempSum += newnode.data.value ; 
				
			}
			
			if ( tempSum + nodeArray[node_number].child_value_sum > nodeArray[node_number].data.value )
			{
				cout<<"------ Error! Sum of values of child nodes cannot be greater than the value of parent node -------"<<endl ; 
				continue ;
			}
			
			for (auto it : nodes)
			{
				it.nodeId = to_string(nodeCounter) ; 
				it.nodeNumber = nodeCounter ; 
				nodeArray[nodeCounter] = it ; 
				nodeCounter++ ; 
				
				nodeArray[node_number].child_value_sum += it.data.value ;
				nodeArray[node_number].childReferenceNodeId += ( it.nodeId + "/" ) ; 
				nodeArray[node_number].childNodes.push_back(it.nodeNumber) ; 
			}
			
			//debug() ; 
			
			
		}else if (choice == 3)
		{
			
			// adding child node of some node 
			
			int node_number ; 
			cout<<"Enter the node number of the parent node : " ; cin>>node_number ;
			
			if (node_number >= nodeCounter)
			{
				cout<<"No Nodes exists with nodeNumber = "<<node_number<<endl ;
				continue ;
			}
			
			double tempSum = 0.0 ; 
			
			cout<<"Details about Child Node "<<endl ;
			struct node newnode = inputNode( nodeArray[node_number].nodeId , nodeArray[node_number].genesisReferenceNodeId ) ;
			tempSum += newnode.data.value ; 
			
			if ( tempSum + nodeArray[node_number].child_value_sum > nodeArray[node_number].data.value )
			{
				cout<<"Error! Sum of values of child nodes cannot be greater than the value of parent node"<<endl ; 
				continue ;
			}
			
			nodeArray[nodeCounter] = newnode ; 
			nodeCounter++ ;
			
			nodeArray[node_number].child_value_sum += newnode.data.value ;
			nodeArray[node_number].childReferenceNodeId += ( newnode.nodeId + "/" ) ; 
			nodeArray[node_number].childNodes.push_back(newnode.nodeNumber) ; 
			
			//debug() ; 
			
		}else if (choice == 4)
		{
			vector<int> temp = longestChain(genesis) ;
			cout<<"The node numbers of longest Chain of genesis node are : " ;
			for (auto it : temp) cout<<it<<" " ; 
			cout<<endl ;  
		}else if ( choice == 5)
		{
			int node_number ; 
			cout<<"Enter Node Number : " ; cin>>node_number ; 
			if (node_number >= nodeCounter)
			{
				cout<<"No Nodes exists with nodeNumber = "<<node_number<<endl ;
				continue ;
			}
			vector<int> temp = longestChain(node_number) ;
			cout<<"The node numbers of longest Chain of given node are : " ;
			for (auto it : temp) cout<<it<<" " ; 
			cout<<endl ;  
		}else if (choice == 6)
		{
			int node1, node2 ; 
			cout<<"Enter node number of first node to be merged : " ; cin>>node1 ; 
			if (node1 >= nodeCounter)
			{
				cout<<"No Nodes exists with nodeNumber = "<<node1<<endl ;
				continue ;
			}
			cout<<"Enter node number of second node to be merged : " ; cin>>node2 ; 
			if (node2 >= nodeCounter)
			{
				cout<<"No Nodes exists with nodeNumber = "<<node2<<endl ;
				continue ;
			}
			
			vector<int> temp1 = longestChain(node1) ;
			vector<int> temp2 = longestChain(node2) ; 
			
			if (temp1.size() >= temp2.size())
				swap(node1,node2) ;
			
			// node1 is merged into node2
			nodeArray[node2].data.value += nodeArray[node1].data.value ; 
			for (auto it : nodeArray[node1].childNodes)
			{
				nodeArray[node2].childNodes.push_back(it) ; 
			}	
			
			nodeArray[stoi( nodeArray[node1].referenceNodeId )].childNodes.erase( find( nodeArray[stoi( nodeArray[node1].referenceNodeId )].childNodes.begin() ,
														nodeArray[stoi( nodeArray[node1].referenceNodeId )].childNodes.end() , node1) ) ;  
			
			cout<<"Both Nodes merged Successfully"<<endl ; 
			
		}else if (choice == 7)
		{
			int node_number ; 
			cout<<"Enter Node Number of node to be edited : " ; cin>>node_number ; 
			if (node_number >= nodeCounter)
			{
				cout<<"No Nodes exists with nodeNumber = "<<node_number<<endl ;
				continue ;
			}
			
			cout<<"The Current value of node is : "<<nodeArray[node_number].data.value<<endl ;
			
			double value ; 
			cout<<"Enter new value for the node : " ; cin>>value ;
			
			if (value < nodeArray[node_number].child_value_sum )
			{
				cout<<"---- Error! The value of node cannot be less than child value sum"<<endl ;
				continue ;
			}else
			{
				if (node_number != genesis)
				{
					double temp = nodeArray[stoi(nodeArray[node_number].referenceNodeId)].child_value_sum ;
					double parentval = nodeArray[stoi(nodeArray[node_number].referenceNodeId)].data.value ;
					
					temp -= nodeArray[node_number].data.value ;
					temp += value ; 
					
					if (parentval < temp)
					{
						cout<<"--- Error! The value of node must satisfy his parent sum condition"<<endl ;
						continue ;
					}
					
					nodeArray[stoi(nodeArray[node_number].referenceNodeId)].child_value_sum = temp ;
					
				}
				nodeArray[node_number].data.value = value ; 
			}
			
			
		}
		
	}
	
	return 0 ;
}
