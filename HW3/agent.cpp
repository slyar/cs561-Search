/*
CSCI 561 - HW3
Slyar
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <set>

#define RIGHT 1
#define LEFT -1

using namespace std;


struct element
{
	int attr;
	string predicate;
	string argument1;
	string argument2;
};

struct item
{
	bool isFact;
	vector<element> clause;
};

element split(string str, int attr)
{
	element re = {attr, "", "", ""};
	int j = 0;
	for (int i = 0; i < str.size(); ++i)
	{
		if (str[i] == '(')
		{
			re.predicate = str.substr(j, i);
			j = i+1;
		}
		else if (str[i] == ',')
		{
			re.argument1 = str.substr(j, i-j);
			j = i+1;
		}
		else if (str[i] == ')')
		{
			re.argument2 = str.substr(j, i-j);
		}
	}
	return re;
}

bool isEqual(const element & l, const element & r)
{
	if ((l.predicate == r.predicate) && (l.argument1 == r.argument1) && (l.argument2 == r.argument2))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool backwardChain(vector<item> & kb, element q)
{
	// find fact
	for (vector<item>::iterator it = kb.begin(); it != kb.end(); it++)
	{
		if (it->isFact && (isEqual(q, it->clause[0])))
		{
			return true;
		}
	}

	bool testing;
	for (vector<item>::iterator it = kb.begin(); it != kb.end(); it++)
	{
		if (!it->isFact)
		{
			for (vector<element>::iterator eright = it->clause.begin(); eright != it->clause.end(); eright++)
			{
				if ((eright->attr == RIGHT) && (isEqual(*eright, q)))
				{
					testing = true;
					for (vector<element>::iterator eleft = it->clause.begin(); eleft != it->clause.end(); eleft++)
					{
						if ((eleft->attr == LEFT) && (backwardChain(kb, *eleft) == false))
						{
							testing = false;
							break;
						}
					}
					if (testing)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

int main()
{
	ifstream ifile;
	ofstream ofile;
	ifile.open("input.txt");
	
	int N;

	// input from the file
	vector<string> in;
	string qstring;
	string tmp;

	ifile >> qstring;
	ifile >> N;

	for (int i = 0; i < N; ++i)
	{
		ifile >> tmp;
		in.push_back(tmp);
	}
	ifile.close();


	// Unify the input
	vector<item> kb;
	// save all possible variables
	set<string> variables;

	// process query clause
	element query = split(qstring, 0);
	if (query.argument1 != "x" && query.argument1 != "")
	{
		variables.insert(query.argument1);
	}
	if (query.argument2 != "x" && query.argument2 != "")
	{
		variables.insert(query.argument2);
	}

	// process input kb
	for (vector<string>::iterator it = in.begin(); it != in.end(); it++)
	{
		item tmpItem;
		element tmpElement;
		string src = *it;
		if (src.find("=>") != std::string::npos)
		{
			int j = 0;
			tmpItem.isFact = false;
			for (int i = 0; i <= src.size(); ++i)
			{
				if (src[i] == '&')
				{
					tmpElement = split(src.substr(j, i-j), LEFT);
					tmpItem.clause.push_back(tmpElement);
					j = i+1;
				}
				else if (src[i] == '=')
				{
					tmpElement = split(src.substr(j, i-j), LEFT);
					tmpItem.clause.push_back(tmpElement);
					j = i+2;
				}
				else if (i == src.size())
				{
					tmpElement = split(src.substr(j, i-j), RIGHT);
					tmpItem.clause.push_back(tmpElement);
				}

				if (tmpElement.argument1 != "x" && tmpElement.argument1 != "")
				{
					variables.insert(tmpElement.argument1);
				}
				if (tmpElement.argument2 != "x" && tmpElement.argument2 != "")
				{
					variables.insert(tmpElement.argument2);
				}
			}
		}
		else
		{
			tmpItem.isFact = true;
			tmpElement = split(src, LEFT);
			if (tmpElement.argument1 != "x" && tmpElement.argument1 != "")
			{
				variables.insert(tmpElement.argument1);
			}
			if (tmpElement.argument2 != "x" && tmpElement.argument2 != "")
			{
				variables.insert(tmpElement.argument2);
			}
			tmpItem.clause.push_back(tmpElement);
		}
		kb.push_back(tmpItem);
	}

	// create new kb
	vector<item> newkb;
	for (vector<item>::iterator it = kb.begin(); it != kb.end(); it++)
	{
		for (set<string>::iterator re = variables.begin(); re != variables.end(); re++)
		{
			item newItem = *it;
			bool replaced = false;

			for (vector<element>::iterator e = newItem.clause.begin(); e != newItem.clause.end(); e++)
			{
				if (e->argument1 == "x")
				{
					replaced = true;
					e->argument1 = *re;
				}

				if (e->argument2 == "x")
				{
					replaced = true;
					e->argument2 = *re;
				}
			}

			if (replaced)
			{
				newkb.push_back(newItem);
			}
			else
			{
				newkb.push_back(newItem);
				break;
			}
		}
	}


	ofile.open("output.txt");
	if (backwardChain(newkb, query))
	{
		ofile << "TRUE" << endl;
	}
	else
	{
		ofile << "FALSE" << endl;
	}

	ofile.close();

	return 0;
}
