#include <iostream>
#include <queue>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <deque>
#include <list>

#define MAX 1001
#define INF 0x7ffffff

using namespace std;

struct vertex
{
	int number;
	string name;
	int cost;
};

int alg;
int source;
int destination;
int rValue;

int numberOfFriend;
std::vector<string> nameOfFriend;
std::vector<string> pathLog;

int martix[MAX][MAX];
bool visited[MAX];
int parent[MAX];
int dis[MAX];
int total_cost = 0;

bool openedFlag[MAX];
bool closedFlag[MAX];

ifstream ifile;
ofstream ofile;

list<vertex> opened;
list<vertex> closed;

int DFS();
int BFS();
int UCS();
int getNumber(string);
string getName(int);
void printPath(int);
void printLog();
void Input();
void output();
bool dfs_compare(const vertex&, const vertex&);
bool ucs_compare(const vertex&, const vertex&);
struct compare;

int main(int argc, char const * argv[])
{
	Input();

	if (alg == 1)
	{
		rValue = BFS();
	}
	else if (alg == 2)
	{
		rValue = DFS();
	}
	else if (alg == 3)
	{
		rValue = UCS();
	}

	output();

	return 0;
}

struct compare  
{  
  bool operator()(const string& left, const string& right)  
  {  
      return left < right;  
  }  
};

bool dfs_compare(const vertex& left, const vertex& right)  
{  
  if (left.cost == right.cost)
  {
  	return left.name < right.name;	
  }
  else
  {
  	return left.cost == right.cost;
  }
}

int DFS()
{
	list<vertex>::iterator it;

	vertex current;
	vertex child;

	vertex start;
	start.number = source;
	start.name = getName(source);
	start.cost = 0;
	parent[start.number] = -1;
	opened.push_front(start);

	for (int i = 0; i < numberOfFriend; ++i)
	{
		openedFlag[i] = false;
		closedFlag[i] = false;
		parent[i] = -1;
	}

	while (1)
	{
		// quit if not find dest
		if (opened.empty())
		{
			return -1;
		}

		// current node
		current = opened.front();

		// record the visit log
		pathLog.push_back(current.name);

		// visit node
		opened.pop_front();
		openedFlag[current.number] = false;

		// return cost if find dest
		if (current.number == destination)
		{
			return current.cost;
		}

		// search children
		for (int i = 0; i < numberOfFriend; ++i)
		{
			// find child
			if (martix[current.number][i])
			{
				child.number = i;
				child.name = getName(child.number);
				child.cost = current.cost + 1;

				// if no node in opened or closed has child's state
				//cout << current.name << "->" << getName(i) << " " << openedFlag[i] << " " << closedFlag[i] << endl;
				if (!openedFlag[i] && !closedFlag[i])
				{
					openedFlag[i] = true;
					
					opened.push_front(child);
					parent[child.number] = current.number;
				}
				// else if there exists child in opened 
				else if (openedFlag[i])
				{
					// find the child in opened
					for (it = opened.begin(); it != opened.end(); it++)
					{
						if (it->number == i)
						{
							break;
						}
					}
					// update child
					if (child.cost < it->cost)
					{
						openedFlag[it->number] = false;
						opened.erase(it);
						openedFlag[child.number] = true;
						opened.push_front(child);
						parent[child.number] = current.number;
					}
				}
				//else if there exists child in closed
				else if (closedFlag[i])
				{
					// find the child in closed
					for (it = closed.begin(); it != closed.end(); it++)
					{
						if (it->number == i)
						{
							break;
						}
					}

					// update child
					if (child.cost < it->cost)
					{
						closedFlag[it->number] = false;
						closed.erase(it);
						openedFlag[child.number] = true;
						opened.push_front(child);
						parent[child.number] = current.number;
					}
				}
			}
		}

		// close current node
		closedFlag[current.number] = true;
		closed.push_front(current);

		// keep priority queue
		opened.sort(dfs_compare);
	}
}
 
bool ucs_compare(const vertex& left, const vertex& right)  
{  
  if (left.cost == right.cost)
  {
  		return left.name < right.name;	
  }
  else
  {
  		return left.cost < right.cost;
  }
}

int UCS()
{
	list<vertex>::iterator it;

	vertex current;
	vertex child;

	vertex start;
	start.number = source;
	start.name = getName(source);
	start.cost = 0;
	parent[start.number] = -1;
	opened.push_front(start);

	for (int i = 0; i < numberOfFriend; ++i)
	{
		openedFlag[i] = false;
		closedFlag[i] = false;
		parent[i] = -1;
	}

	while (1)
	{
		// quit if not find dest
		if (opened.empty())
		{
			return -1;
		}

		// current node
		current = opened.front();

		// record the visit log
		pathLog.push_back(current.name);

		// visit node
		opened.pop_front();
		openedFlag[current.number] = false;

		// return cost if find dest
		if (current.number == destination)
		{
			return current.cost;
		}

		// search children
		for (int i = 0; i < numberOfFriend; ++i)
		{
			// find child
			if (martix[current.number][i])
			{
				child.number = i;
				child.name = getName(child.number);
				child.cost = current.cost + martix[current.number][i];

				// if no node in opened or closed has child's state
				if (!openedFlag[i] && !closedFlag[i])
				{
					openedFlag[i] = true;
					
					opened.push_back(child);
					parent[child.number] = current.number;
				}
				// else if there exists child in opened 
				else if (openedFlag[i])
				{
					// find the child in opened
					for (it = opened.begin(); it != opened.end(); it++)
					{
						if (it->number == i)
						{
							break;
						}
					}

					// update child
					if (child.cost < it->cost)
					{
						openedFlag[it->number] = false;
						opened.erase(it);
						openedFlag[child.number] = true;
						opened.push_back(child);
						parent[child.number] = current.number;
					}
				}
				// else if there exists child in closed
				else if (closedFlag[i])
				{
					// find the child in closed
					for (it = closed.begin(); it != closed.end(); it++)
					{
						if (it->number == i)
						{
							break;
						}
					}

					// update child
					if (child.cost < it->cost)
					{
						closedFlag[it->number] = false;
						closed.erase(it);
						openedFlag[child.number] = true;
						opened.push_back(child);
						parent[child.number] = current.number;
					}
				}
			}
		}

		// close current node
		closedFlag[current.number] = true;
		closed.push_back(current);

		// keep priority queue
		opened.sort(ucs_compare);
	}
}

int BFS()
{
	for (int i = 0; i < numberOfFriend; ++i)
	{
		visited[i] = false;
		parent[i] = -1;
		dis[i] = INF;
	}

	int current;
	deque <int> bfs_queue;
	visited[source] = true;
	dis[source] = 0;

	bfs_queue.push_back(source);

	priority_queue<string, vector<string>, compare> pri_queue;

	while (1)
	{
		if (bfs_queue.empty())
		{
			return -1;
		}

		// make queue have alp order
		current = dis[bfs_queue.front()];
		while (!bfs_queue.empty() && current == dis[bfs_queue.front()])
		{
			pri_queue.push(getName(bfs_queue.front()));
			bfs_queue.pop_front();
		}

		while (!pri_queue.empty())
		{
			bfs_queue.push_front(getNumber(pri_queue.top()));
			pri_queue.pop();
		}

		// run BFS
		current = bfs_queue.front();
		pathLog.push_back(getName(current));
		bfs_queue.pop_front();
		if (current == destination)
		{
			return dis[current];
		}

		for (int i = 0; i < numberOfFriend; ++i)
		{
			if (martix[current][i])
			{
				// If not visited or has smallar cost
				if (!visited[i])
				{	
					visited[i] = true;
					parent[i] = current;
					dis[i] = dis[current] + 1;
					bfs_queue.push_back(i);
				}
			}
		}

	}
}

void printPath(int w)
{
	if (parent[w] != -1)
	{
		printPath(parent[w]);
		ofile << "-" << getName(w);
		total_cost += martix[w][parent[w]];
	}
	else
	{
		ofile << getName(w);
	}
}

void printLog()
{
	for (vector<string>::iterator it = pathLog.begin(); it != pathLog.end(); it++)
	{
		if (it != pathLog.begin())
		{
			ofile << "-";
		}
		ofile << *it;
	}
	ofile << endl;
}

void Input()
{
	// opened input file
	ifile.open("input.txt");

	string srcNmae;
	string destName;
	string tmp;

	ifile >> alg;
	ifile >> srcNmae;
	ifile >> destName;
	ifile >> numberOfFriend;

	// read the friend notes
	for (int i = 0; i < numberOfFriend; ++i)
	{
		ifile >> tmp;
		nameOfFriend.push_back(tmp);
	}

	// find the position of source and destination
	vector<string>::iterator it;

	it = find(nameOfFriend.begin(), nameOfFriend.end(), srcNmae);
	source = it - nameOfFriend.begin();

	it = find(nameOfFriend.begin(), nameOfFriend.end(), destName);
	destination = it - nameOfFriend.begin();

	// read the martix
	for (int i = 0; i < numberOfFriend; ++i)
	{
		for (int j = 0; j < numberOfFriend; ++j)
		{
			ifile >> martix[i][j];
		}
	}

	// close input file
	ifile.close();
}

void output()
{
	ofile.open("output.txt");

	if (rValue < 0)
	{
		printLog();
		ofile << "NoPathAvailable" << endl;
		ofile << 0 << endl;
	}
	else
	{
		printLog();
		printPath(destination);
		ofile << endl;
		ofile << total_cost << endl;
	}

	ofile.close();
}

int getNumber(string name)
{
	vector<string>::iterator it;

	it = find(nameOfFriend.begin(), nameOfFriend.end(), name);
	int number = it - nameOfFriend.begin();

	return number;
}

string getName(int number)
{
	return nameOfFriend[number];
}
