#include"graph.h"
#include<fstream>
#include<iostream>
#include<set>
extern float MINSCORE;
extern float Tolerance;

using namespace std;
/*
*Input:file path
*	create the graph from path
*/

graph::graph() {
	NID = 0;
	nodesize = 0;
	edgesize = 0;
	nbs = NULL;
	sizes = NULL;
}

void graph::init(const string & path) {//********************


	ifstream fin(path.c_str());
	
	if (!fin) {
		cerr << "Error: Cannot open the edge file!" << endl;
		return;
	}

	string temp;
	vector<list<intFloat> > gph;
	cout << "Reading edges file..." << endl;
	while (getline(fin, temp)) {
		if ((temp.length() <= 1) || (temp.c_str()[0] == '#')){//*****skip line*****/
			//cout << "skip line\t" << temp << endl;
			continue;
		}
		int begin, end;
		float weight;
		getEdge(gph, temp, begin, end, weight);

		intFloat tempIntFloat;
		tempIntFloat.id = end;
		tempIntFloat.weight = weight;
		gph[begin].push_back(tempIntFloat);

		tempIntFloat.id = begin;
		gph[end].push_back(tempIntFloat);
		edgesize += 2;
	}

	arraylize(gph);

	for (int i = 0; i < nodesize; i++) {
		volumesum += volumes[i];
	}

}

graph::~graph() {//******************
	if (nbs != NULL) {
		for (int i = 0; i < NID; i++) {
			if (nbs[i] != NULL) {
				delete nbs[i];
				nbs[i] = NULL;
			}
			if (ws[i] != NULL) {
				delete ws[i];
				ws[i] = NULL;
			}
		}
		delete[] nbs;
		delete[] ws;
		nbs = NULL;
		ws = NULL;
	}
	if (sizes != NULL) {
		delete[] sizes;
		sizes = NULL;
	}

	if (volumes != NULL) {
		delete [] volumes;
		volumes = NULL;
	}

}

/*
* input: a string "beginNode endNode" or "beginNode endNode weight"
* output: beginNodeID and endNodeID
*/
void graph::getEdge(vector<list<intFloat> >& gph, string &line,int &begin,int &end, float &weight) {//*********

	const char *chs = line.c_str();
	string sbegin;
	string send;
	std::vector<string> ss;
	split(ss,line, '\t');
	if(ss.size()<2){
		begin = -1;
		end  = -1;
		return;
	}

	begin = getID(gph,ss[0]);
	end = getID(gph,ss[1]);
	if (ss.size() >= 3)
		weight = atoi(ss[2].c_str());
	else
		weight = 1.0;
}


/*
*Input: string
*Output: node ID
*If exist, return id
*else create the map, and create the neiborhood list
*/
int graph::getID(vector<list<intFloat> >& gph,string &str) {//***********
	if (string2id.find(str) != string2id.end())
		return string2id.find(str)->second;
	string2id[str] =  NID;
	vecId2string.push_back(str);
	gph.push_back(list<intFloat>());
	return NID++;
}

/*
*Input : vector of list of int stored neiborhoods
*Convert to array
*/
void graph::arraylize(vector<list<intFloat> >& gph) {//**********************
	nodesize = gph.size();
	nbs = new int*[gph.size()];
	memset(nbs, 0, sizeof(int*)*gph.size());

	ws = new float*[gph.size()];
	memset(ws,0,sizeof(float*)*gph.size());

	sizes = new int[gph.size()];
	memset(sizes, 0, sizeof(int)*gph.size());

	volumes = new float[gph.size()];
	memset(volumes, 0, sizeof(float)*gph.size());


	for (int i = 0; i < gph.size(); i++) {
		list<intFloat> &nblist = gph[i];
		nbs[i] = new int[nblist.size()];
		ws[i] = new float[nblist.size()];
		sizes[i] = nblist.size();
		float sumv =0;
		int j = 0;
		for (list<intFloat>::iterator lite = nblist.begin(); lite != nblist.end(); lite++,j++) {
			nbs[i][j] = lite->id;
			ws[i][j] = lite->weight;
			sumv+=ws[i][j];
		}
		volumes[i] = sumv;
	}
}

/*
*Input: string
*Output: Node ID if exist, else return -1;
*/
int graph::getID(string str) {//*******************
	if (string2id.find(str) == string2id.end())
		return -1;
	return string2id.find(str)->second;
}

string graph::getStr(int id) {//************
	if (id<0 || id>=nodesize)
		return "";
	return vecId2string[id];
}

/*
* input: x and alpha 
* output:alpha*P*x- through pointer
* calculate alpha*P*x
*/
void graph::multi(map<int,float>& result, map<int,float> &x, float alpha) {//**************
	result.clear();
	for (map<int,float>::iterator mite = x.begin(); mite != x.end(); mite++) {
		int cnodeid = mite->first;
		if (abs(mite->second) < Tolerance )
			continue;
		int *nb = nbs[cnodeid];
		float *weight = ws[cnodeid];
		for (int i = 0; i < sizes[cnodeid]; i++) {
			if (result.find(nb[i]) == result.end()) {
				result[nb[i]] = alpha*mite->second* weight[i] / volumes[cnodeid];
			}
			else
				result[nb[i]] += alpha*mite->second *weight[i] / volumes[cnodeid];
		}
	}
}

/*
* input: x 
* output:P*x- through pointer
* calculate P*x
*/
void graph::multi(map<int,float>& result, map<int,float> &x) {//*************
	multi(result, x, 1.0);
}


void graph::printOriginal(ofstream &fout, map<int, float>& out, const char* str) {//*****************
	fout << str << endl;
	for (map<int, float>::iterator mite = out.begin(); mite != out.end(); mite++) {
		fout << vecId2string[mite->first] << "\t" << mite->second << endl;
	}
	fout << endl;
}


/*
*Input: indexs: ids;
*		size: also the length of indexs, return 0 if size ==0;
*/
int graph::findCommunity(int* indexs, int size) {//*********************
	if (size <= 0)
		return 0;

	int bestsplit = 0;
	float best = 0;
	set<int> cmtyCandidate;

	//init
	//S = {indexs[0]}
	//hS = V-S;

	//ahs = # of edges from hS to V
	//as = # of edges from S to V
	//s2hs # edge from S to hS.

	float ahs = volumesum;

	//initially, S = {indexs[0]},
	float as = volumes[indexs[0]];
	ahs = ahs - as;

	// since there are no self loop in the graph, all edge starting from S link to hS;
	float s2hs = as;
	int bestindex = 1;
	cmtyCandidate.insert(indexs[0]);
	//calculate the conductance
	float bestsocre = 1.0;

	//add another node to S. Then, 
	for (int i = 1; i < size; i++) {

		//since move index[i] from hs to S. adjust as and ahs
		as += volumes[indexs[i]];
		ahs -= volumes[indexs[i]];

		float i2s = 0;
		// add edge from index[j] to hS
		for (int k = 0; k < sizes[indexs[i]]; k++) {
			int nb = nbs[indexs[i]][k];
			if (cmtyCandidate.find(nb) != cmtyCandidate.end()) {
				i2s += ws[indexs[i]][k];
			}
		}
		s2hs -= i2s;
		s2hs += (volumes[indexs[i]] - i2s);

		float tempscore = 1.0*s2hs / MYmin(as, ahs);
		if (tempscore < bestsocre) {
			bestsocre = tempscore;
			bestindex = i;
		}
		cmtyCandidate.insert(indexs[i]);
	}

	return bestindex;
}