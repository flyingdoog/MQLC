#pragma once
#ifndef _HEAD_TOOL
#define _HEAD_TOOL
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include <list>
#include <algorithm>
using namespace std;
#define ERROR_CODE -1234567
void addArray(map<int,float>&,float,map<int,float>&,float,map<int,float>&);
void addArray(map<int,float>&, float, map<int,float>&);
void copyArray(map<int,float>& to, map<int,float>&);
void dotArray(map<int,float>&,float);
void dotArray(map<int,float>&, float,map<int,float>&);
float pearsonCorrelation(map<int,float>&, map<int,float>&);
float PospearsonCorrelation(map<int, float>&, map<int, float>&);
float cosSim(map<int, float>&, map<int, float>&);
float CosSim(map<int, float>&, map<int, float>&);
float maxValue(map<int,float>&);
void catArray(map<int, float>&, map<int, float>&);
void catAndNormArray(map<int, float>& to, map<int, float>& from);
void split(vector<string>& result, string &origin, char c);
bool convergeArray(const map<int, float>& m1, const map<int, float>& m2, float thre);
void randomShuffle(int* shuffleIndex, int S);
int findInShuffle(int* shuffleIndex, int S, int i_OR);
#define MYmin(a,b)        (((a) < (b)) ? (a) : (b))

#endif // !_HEAD_TOOL
