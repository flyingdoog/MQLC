#include"tools.h"
#include<fstream>
#define max(x,y) ((x)>(y)? (x):(y))
#define FLT_MAX 99999999
extern float MINSCORE;
/*
* results = alpha*m1+beta*m2; result must be empty
*/
void addArray(map<int,float> &results, float alpha, map<int,float>& m1, float beta, map<int,float>&m2) {//**********
	results.clear();
	map<int, float>::iterator mite1 = m1.begin();
	//map<int, float>::iterator mite2 = m2.begin();
	map<int, float>::iterator index;
	dotArray(results,beta,m2);
	for(; mite1 != m1.end(); mite1++) {
		index = results.find(mite1->first);
		if (index == results.end()) {
			results.insert(map<int, float>::value_type(mite1->first, alpha*mite1->second));
		}
		else
			index->second += alpha*mite1->second;
	}
}

/*
* m1 = m1 + beta*m2;
*/
void addArray(map<int,float> &m1, float beta, map<int,float>&m2) {//***************
	
	map<int, float>::iterator mite = m2.begin();
	map<int, float>::iterator index;
	
	for (; mite != m2.end(); mite++) {
		index = m1.find(mite->first);
		if (index == m1.end()) {
			m1.insert(map<int, float>::value_type(mite->first, beta*mite->second));
		}
		else
			index->second += beta*mite->second;
	}
}

/*
* m1 = m1*alpha
*/
void dotArray(map<int,float>& m1, float alpha) {//*******
	for (map<int, float>::iterator mite = m1.begin(); mite != m1.end(); mite++) {
		mite->second *= alpha;
	}
}

/*
* res = m1*alpha
*/
void dotArray(map<int,float>& res, float alpha, map<int,float>& m1) {//*************
	res.clear();
	for (map<int, float>::iterator mite = m1.begin(); mite != m1.end(); mite++) {
		res.insert(map<int, float>::value_type(mite->first, mite->second*alpha));
	}

}

/*
* 
*/
float pearsonCorrelation(map<int,float>& m1, map<int,float>& m2) {//************************
	float sumx=0, sumy=0,sumx2=0,sumy2=0,sumxy=0;
	int n = 0;
	map<int, float>::iterator mite1 = m1.begin();
	map<int, float>::iterator mite2 = m2.begin();

	while (mite1 != m1.end() && mite2 != m2.end()) {
		if (mite1->first == mite2->first) {
			sumxy += mite1->second*mite2->second;
			sumx += mite1->second;
			sumx2 += (mite1->second)*(mite1->second);
			sumy += mite2->second;
			sumy2 += (mite2->second)*(mite2->second);

			mite1++;
			mite2++;
		}
		else if (mite1->first > mite2->first) {
			sumy += mite2->second;
			sumy2 += (mite2->second)*(mite2->second);
			mite2++;
		}
		else {
			sumx += mite1->second;
			sumx2 += (mite1->second)*(mite1->second);
			mite1++;
		}
		n++;
	}
	while(mite1 != m1.end()) {
		sumx += mite1->second;
		sumx2 += (mite1->second)*(mite1->second);
		n++;
		mite1++;
	}
	while (mite2 != m2.end()) {
		sumy += mite2->second;
		sumy2 += (mite2->second)*(mite2->second);
		n++;
		mite2++;
	}

	float dev = sqrt(n*sumx2 - sumx*sumx)*sqrt(n*sumy2 - sumy*sumy);

	//If their vectors are same and with uniform values, return 1
	if (dev == 0) {
		return 1; 
	}
	return (n*sumxy - sumx*sumy) / dev;

}

float PospearsonCorrelation(map<int, float>& m1, map<int, float>& m2) {//*************
	float res1 = pearsonCorrelation(m1, m2);
	return max(res1, 0);
}


float cosSim(map<int, float>& m1, map<int, float>& m2) {//************************
	float sumx = 0, sumy = 0, sumx2 = 0, sumy2 = 0, sumxy = 0;
	int n = 0;
	map<int, float>::iterator mite1 = m1.begin();
	map<int, float>::iterator mite2 = m2.begin();

	while (mite1 != m1.end() && mite2 != m2.end()) {
		if (mite1->first == mite2->first) {
			sumxy += mite1->second*mite2->second;
			sumx += mite1->second;
			sumx2 += (mite1->second)*(mite1->second);
			sumy += mite2->second;
			sumy2 += (mite2->second)*(mite2->second);

			mite1++;
			mite2++;
		}
		else if (mite1->first > mite2->first) {
			sumy += mite2->second;
			sumy2 += (mite2->second)*(mite2->second);
			mite2++;
		}
		else {
			sumx += mite1->second;
			sumx2 += (mite1->second)*(mite1->second);
			mite1++;
		}
		n++;
	}
	while (mite1 != m1.end()) {
		sumx += mite1->second;
		sumx2 += (mite1->second)*(mite1->second);
		n++;
		mite1++;
	}
	while (mite2 != m2.end()) {
		sumy += mite2->second;
		sumy2 += (mite2->second)*(mite2->second);
		n++;
		mite2++;
	}

	float dev = sqrt(n*sumx2 - sumx*sumx)*sqrt(n*sumy2 - sumy*sumy);

	//If their vectors are same and with uniform values, return 1
	if (dev == 0) {
		return 1;
	}

	return sumxy / (sqrt(sumx2)*sqrt(sumy2));

}

float CosSim(map<int, float>& m1, map<int, float>& m2) {//*************
	float res1 = cosSim(m1, m2);
	return max(res1, 0);
}

float maxValue(map<int,float>& m1) {//********
	float max = -FLT_MAX;
	for (map<int, float>::iterator mite = m1.begin(); mite != m1.end(); mite++) {
		if (mite->second > max)
			max = mite->second;
	}
	return max;
}

void copyArray(map<int,float> &to, map<int,float>&from) {//*******************
	to.clear();
	for (map<int,float>::iterator mite = from.begin();mite!=from.end();mite++)
		to[mite->first] = mite->second;

}

void catArray(map<int, float>& to, map<int, float>& from) {//*******************
	map<int, float>().swap(to);
	for (map<int, float>::iterator mite = from.begin(); mite != from.end(); mite++)
		if(mite->second>MINSCORE)
			to[mite->first] = mite->second;
}


void catAndNormArray(map<int, float>& to, map<int, float>& from) {//*******************
	map<int, float>().swap(to);
	float sum = 0;
	for (map<int, float>::iterator mite = from.begin(); mite != from.end(); mite++)
		if (mite->second > MINSCORE)
			sum += mite->second;

	for (map<int, float>::iterator mite = from.begin(); mite != from.end(); mite++)
		if (mite->second>MINSCORE)
			to[mite->first] = mite->second/sum;
}

void split(vector<string>& result, string &origin, char c) {//****************
	if (origin.length() <= 1)
		return;
	int begin = 0;
	int next = 0;
	int end = origin.length();
	while (next<end) {
		if (origin[next] == c) {
			if (next - begin > 0)
				result.push_back(origin.substr(begin, (next - begin)));
			begin = next + 1;
		}
		next++;
	}
	if (next - begin > 0)
		result.push_back(origin.substr(begin, (next - begin)));
}


bool convergeArray(const map<int, float>& m1, const map<int, float>& m2, float thre) {//**********


	map<int, float>::const_iterator mite1 = m1.begin();
	map<int, float>::const_iterator mite2 = m2.begin();
	float maxi = 0;
	while (mite1 != m1.end() && mite2 != m2.end()) {
		if (mite1->first == mite2->first) {
			if (abs(mite1->second - mite2->second) > thre)
				return false;
			mite1++;
			mite2++;
		}
		else if (mite1->first < mite2->first) {
			if (abs(mite1->second) > thre)
				return false;
			mite1++;
		}
		else {
			if (abs(mite2->second) > thre)
				return false;
			mite2++;
		}
	}

	while (mite1 != m1.end()) {
		if (abs(mite1->second) > thre)
			return false;
		mite1++;
	}
	while (mite2 != m2.end()) {
		if (abs(mite2->second) > thre)
			return false;
		mite2++;
	}

	return true;
}

void randomShuffle(int* shuffleIndex, int S) {
	
	vector<int> vShuffleIndex;
	for (int i = 0; i < S; i++) {
		vShuffleIndex.push_back(i);
	}
	random_shuffle(vShuffleIndex.begin(), vShuffleIndex.end());
	int i = 0;

	for (vector<int>::iterator it = vShuffleIndex.begin(); it != vShuffleIndex.end(); ++it) {
		shuffleIndex[i] = *it;
		i++;
	}
}
int findInShuffle(int* shuffleIndex, int S, int i_OR) {
	int *p;
	p = find(shuffleIndex, shuffleIndex + S, i_OR);
	if (p != shuffleIndex + S) {
		return distance(shuffleIndex, p);
	}
	else {
		cerr << "\nERROR: Cannot get back the original index of query node!" << endl;
		return -1;
	}
}
