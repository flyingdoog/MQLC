#pragma once
#ifndef _HEAD_GRAPH
#define _HEAD_GRAPH
#include"tools.h"


typedef struct {
	int id;
	float weight;
}intFloat;

class graph {

private:
	//after arraylized nned to optimize
	//*************int sizeOfWholeEdgeSet
	//*************float **weights
	//*************float *volumes
	int **nbs;
	int * sizes;
	float **ws;
	float *volumes;
	float volumesum;


	int nodesize;
	
	map<string, int> string2id;
	map<int, string> id2string;//reindex
	int *strings;//id2string;
	
	vector<string> vecId2string;

private:
	void getEdge(vector<list<intFloat> >&,string &, int&, int&, float &weight);
	int getID(vector<list<intFloat> >&, string&);
	void arraylize(vector<list<intFloat> >&);

public:
	int NID;
	int edgesize;
	graph();
	~graph();
	void init(const string&);
	int getID(string);
	string getStr(int);
	void multi(map<int,float>&, map<int,float>&, float);
	void multi(map<int,float>&, map<int,float>&);
	void printOriginal(ofstream &fout,map<int, float>&, const char*);
	int findCommunity(int* indexs, int size);
};




#endif // !_HEAD_TOOL

