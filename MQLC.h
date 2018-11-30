#pragma once
#ifndef _HEAD_MQLC
#define _HEAD_MQLC
#include"tools.h"
#include"graph.h"
#include"Matrix.h"
#include<fstream>
#include<time.h>
float Tolerance = 1e-6;
float alpha = 0.1;
float beta = 0.6;
float gamma_W = 0.3;
int K = 5;
float epsilon = 1e-3;
float theta = 0.0;
float SIMITHRES = 0.8;
float MINSCORE = epsilon / 2;
int MAXITE = 50;
int topK = 200;
bool usingNb = true;

graph P;
void readQueryNode(vector<int> &seeds, string &path);

#endif // !_HEAD_MQLC
