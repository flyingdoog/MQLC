// Input: Option:
//		-i: Input file name for edges of the network.
//		-o: Output directory
//		-s: Seedfile
//		-a: paramater: alpha
//		-b: paramater: beta
//		-g: paramater: gamma_W
//		-k: paramater: K
//		-e: epsilon
//		-t: theta
//Output:
//		hatR: outputDirectory/hatR.dat (opt)
//		X:	outputDirectory/Xs.dat (opt)
//		communities:sOutputFolder/communities.dat";
//

#include"MQLC.h"
#include"heapSort.h"
using namespace std;
Heap  heap = Heap(topK);


/*
* Read seeds from path, stored to vector seeds
*/
void readQueryNode(vector<string> &seeds, string &path) {//***************
	ifstream fin(path.c_str());
	if (!fin) {
		cerr << "Error: Cannot open the query nodes file!" << endl;
		return;
	}


	string line;
	const char* chs;
	cout << "\nReading query nodes set..." << endl;
	while (getline(fin, line)) {

		if ((line.length() <= 1) || (line.c_str()[0] == '#')) {//*****skip line*****/
			continue;
		}

		vector<string> ss;
		split(ss, line, '\t');
		for (int i = 0; i < ss.size(); i++) {
			if (ss[i].length() > 0)
				seeds.push_back(ss[i]);
		}
	}
	cout << "Finish reading query nodes set!" << endl;
	cout << "There are " << seeds.size() << " query nodes!" << endl;
}


/*
* Print out cmty results
*/
void printCmty(ofstream & fouti, map<int, float> *xs, int i, int *Q) {
	float temp = xs[i][Q[i]];
	xs[i][Q[i]] = 1.0;
	heap.clear();
	
	for (map<int, float>::iterator mite = xs[i].begin(); mite != xs[i].end(); mite++)
		heap.insert(mite->first, mite->second);

	int *result = heap.getSortedHeap();// result is the index after sorting based on score values in xs[i]

	int size = 0;
	for (; size < topK; size++)
		if (xs[i].find(result[size]) == xs[i].end() || xs[i][result[size]] <= MINSCORE)
			break;

	int bestIndex = P.findCommunity(result, size);

	for (int j = 0; j <= bestIndex; j++) {
		fouti << P.getStr(result[j]) << "\t";
	}
	fouti << endl << endl;

	xs[i][Q[i]] = temp;
}


/*
* output: xs			:	sOutputFolder + "/Xs.dat"
*		: communities	:	sOutputFolder + "/communities.dat";
*		: hatR			:	sOutputFolder + "/hatR.dat"
*/
void MQLC(string & sInputFileNameEdges, string & sInputFileNameSeeds, string & sOutputFolder,
	float alpha, float beta, float gamma_W, int K, float epsilon, float theta) {

	//read edge file
	cout << "\nBegin to read files!" << endl;
	P.init(sInputFileNameEdges);

//	string xpath = sOutputFolder + "/Scores.dat";
	string ipath = sOutputFolder + "/communities.dat";
//	string rpath = sOutputFolder + "/hatR.dat";

//	ofstream foutx(xpath.c_str());
//	ofstream foutr(rpath.c_str());
	ofstream fouti(ipath.c_str());


	cout << "Finish reading edge file!" << endl;
	cout << "There are " << P.NID << " nodes and " << P.edgesize / 2 << " edges" << endl;

	vector<string> Qstr;// query node
	readQueryNode(Qstr, sInputFileNameSeeds);
	if (Qstr.size() == 0) {
		return;
	}

	int S = 0;
	
	int *shuffleIndex = new int [Qstr.size()];
	randomShuffle(shuffleIndex, Qstr.size());

	int *Q = new int[Qstr.size()];
	for (int i = 0; i < Qstr.size(); i++) {
		int id = P.getID(Qstr[shuffleIndex[i]]);
		if (id == -1) {
			cerr << "ERROR:\t" << Qstr[shuffleIndex[i]] << "\tnot found" << endl;
			return;
		}
		else {
//			cout << Qstr[i] << "\t"<<id << "\t" << endl;
			Q[S] = id;
			S++;
		}
	}

	if (S == 0) { 
		cout << "Query nodes set is empty!" << endl;
		return;
	}
	
	cout << "\nWalkers begin to explore the graph!" << endl;

	map<int, float> * xs = new map<int, float>[S];
	map<int, float> * xsnext = new map<int, float>[S];
	map<int, float> * vs = new map<int, float>[S];
	map<int, float> ** keyP = new map<int, float>*[K];
	for (int i = 0; i < K; i++)
		keyP[i] = new map<int, float>[S];
	map<int, float> * hxs = new map<int, float>[S];

	//initialization
	int t = 0;
	for (int i = 0; i < S; i++) {
		xs[i][Q[i]] = 1.0;
		hxs[i][Q[i]] = 1.0;
		vs[i][Q[i]] = 1.0;
		for (int k = 0; k < K; k++) {
			keyP[k][i][Q[i]] = 1.0;
		}
	}

	
	//flags 
	bool* convergence = new bool[S];
	for (int i = 0; i < S; i++)
		convergence[i] = false;

	int* mergeTo = new int[S];
	memset(mergeTo, -1, sizeof(int)*S);

	DenseMatrix fR(S, S);


	while (1) {
		//calculate x_t+1
		for (int i = 0; i < S; i++) {
			
			if (mergeTo[i] >= 0) {
				copyArray(hxs[i], hxs[mergeTo[i]]);
				continue;
			}
			if (convergence[i] && mergeTo[i] == -1) {
				copyArray(hxs[i], xs[i]);
				continue;
			}
			map<int, float> alphaPx;
			P.multi(alphaPx, xs[i], alpha);// alphaPx = \alpha*P^T*xs^(t)
			addArray(alphaPx, (1 - alpha), vs[i]);// alphaPx = alphaPx + (1-\alpha)*v^(t)
			map<int, float> &hxt_1 = alphaPx;
			catAndNormArray(hxs[i], hxt_1);// hxs[i]: mediate variable; xs[i]: keep it to check convergence
		}

		//initialize hatR and R
		DenseMatrix hatR(S, S);
		for (int i = 0; i < S; i++) {
			if (mergeTo[i] >= 0) {
				for (int j = i + 1; j < S; j++) {
					hatR.insert(i, j, hatR.getElement(mergeTo[i], j));
					hatR.insert(j, i, hatR.getElement(i, j));
				}
				continue;
			}
			for (int j = i + 1; j < S; j++) {
				if (mergeTo[j] >= 0) {
					if (i == mergeTo[j])
						hatR.insert(i, j, 1);
					else
						hatR.insert(i, j, hatR.getElement(i, mergeTo[j]));
					continue;
				}
				/*float ppc = PospearsonCorrelation(hxs[i], hxs[j]);
				if (ppc > Tolerance)
					hatR.insert(i, j, ppc);*/
				float cosScore = CosSim(hxs[i], hxs[j]);
				if (cosScore > theta)
					hatR.insert(i, j, cosScore);
				hatR.insert(j, i, hatR.getElement(i, j));
			}
		}


		DenseMatrix R(S, S);
		hatR.deepCopyTo(R);
		R.ColumnNormalize();

		//compute \mathbb{x} ()
		for (int i = 0; i < S; i++) {
			if (mergeTo[i] >= 0) {
				int maxindex = mergeTo[i];
				copyArray(xsnext[i], xsnext[maxindex]);//copy one score vectors to another if they are merged 
				continue;
			}
			if (convergence[i] && mergeTo[i] == -1) {
				copyArray(xsnext[i], xs[i]); //stop update if some vector has converged
				continue;
			}

			float columnSumi = R.columnSum(i);
			if (abs(columnSumi - 1) < Tolerance) {
				map<int, float> temp;
				dotArray(temp, 1 - gamma_W, hxs[i]);

				for (int j = 0; j < S; j++) {
					if (R.getElement(j, i) > 0) {
						addArray(temp, gamma_W*R.getElement(j, i), hxs[j]);
					}
				}
				catAndNormArray(xsnext[i], temp);
			}
			else
				catArray(xsnext[i], hxs[i]);
            //Updata v_i^(t+1)			
			//find the max value
			float max = maxValue(hxs[i]);
			map<int, float> keyPNext;
			for (map<int, float>::iterator mite = hxs[i].begin(); mite != hxs[i].end(); mite++) {
				if (abs(mite->second - max) < Tolerance) {
					keyPNext.insert(map<int, float>::value_type(mite->first, 1.0));
				}
			}
			dotArray(keyPNext, 1.0 / keyPNext.size());
			
			int tnext = (t + 1) % K;
			keyP[tnext][i].swap(keyPNext);


			map<int, float> g;
			for (int k = 0; k < K; k++) {
				map<int, float> enext;
				if (usingNb)
					P.multi(enext, keyP[k][i]);
				else
					copyArray(enext, keyP[k][i]);
				addArray(g, 1.0 / K, enext);
			}
			
			map<int, float> vnext;
			float pt = pow(beta, t);

			addArray(vnext, 1 - pt, vs[i], pt, g);
			vnext.swap(vs[i]);
		}


		//merge X according to hatR
		for (int i = 0; i < S; i++) {
			float max = 0;
			int maxindex = 0;
			//find the max, which index is less than mergetTo[i], if has
			int largeIndex = (mergeTo[i] == -1) ? i : mergeTo[i];

			for (int j = 0; j < largeIndex; j++) {
				if (hatR.getElement(j, i) > max && 1 - hatR.getElement(j, i) > Tolerance) {
					max = hatR.getElement(j, i);
					maxindex = j;
				}
			}
			//average and update
			if (max > SIMITHRES) {
				mergeTo[i] = maxindex;
				addArray(xsnext[i], 1.0, xsnext[mergeTo[i]]);
				dotArray(xsnext[i], 0.5);
				copyArray(xsnext[mergeTo[i]], xsnext[i]);
			}
		}

		//check convergence 
		for (int i = 0; i < S; i++) {
			if (mergeTo[i] >= 0) {
				copyArray(xsnext[i], xsnext[mergeTo[i]]);
				convergence[i] = convergence[mergeTo[i]];
				continue;
			}
			if (convergence[i] && mergeTo[i] == -1) {
				copyArray(xsnext[i], xs[i]);
				continue;
			}
			convergence[i] = convergeArray(xsnext[i], xs[i], epsilon);
		}

		//for next iteration
		t += 1;
		map<int, float> *temp;
		temp = xs;
		xs = xsnext;
		xsnext = temp;
		map<int, float>  temp2;
		xsnext->swap(temp2);


		bool done = true;
		for (int i = 0; i < S; i++) {
			if (!convergence[i] && mergeTo[i] == -1) {
				done = false;
				break;
			}

		}

		if (t == MAXITE) {
			done = true;
		}


		if (done) {
			cout << "\nCommunities detected!" << endl;
			hatR.deepCopyTo(fR);
			break;
		}
	}

	cout << "\nPrinting output file..." << endl;

    // print out final reinforcement matrix
	for (int i = 0; i < S; i++) {
		if (mergeTo[i] >= 0) {
			fR.insert(i, mergeTo[i], 1.0);
			fR.insert(mergeTo[i], i, 1.0);
			for (int j = 0; j < S; j++) {
				if (j != mergeTo[i] && j != i) {
					fR.insert(i, j, fR.getElement(mergeTo[i], j));
					fR.insert(j, i, fR.getElement(j, mergeTo[i]));
				}
				else {
					continue;
				}				
			}
		}
		else {
			continue;
		}
	}

	DenseMatrix fR_OR(S, S);
	for (int i_OR = 0; i_OR < S; i_OR++) {
		for (int j_OR = i_OR + 1; j_OR < S; j_OR++) {
			fR_OR.insert(i_OR, j_OR,
				fR.getElement(findInShuffle(shuffleIndex, Qstr.size(), i_OR),
					findInShuffle(shuffleIndex, Qstr.size(), j_OR)));
			fR_OR.insert(j_OR, i_OR, fR_OR.getElement(i_OR, j_OR));
		}
	}
	//fR_OR.print(foutr);


	//find community
	for (int i_OR = 0; i_OR < S; i_OR++) {
		int i = findInShuffle(shuffleIndex, Qstr.size(), i_OR);
		if(i == -1){
			return;
		}
			
		if (mergeTo[i] >= 0) {
			fouti << "# Detected Community for Query node " << i_OR + 1<< ": "<< P.getStr(Q[i])  << "\tMERGES with Query node "
				<< shuffleIndex[mergeTo[i]] + 1 << ": " << P.getStr(Q[mergeTo[i]]) << endl;
			//detect community and print out 
			printCmty(fouti, xs, mergeTo[i], Q);

			//foutx << "# x_" << i_OR + 1 << "\tMERGES with Query node "
			//	<< shuffleIndex[mergeTo[i]] + 1 << "." << endl << endl;
			continue;
		}
		fouti << "# Detected Community for Query node " << i_OR + 1 << ": " << P.getStr(Q[i]) << endl;
		// detect community and print out 
		printCmty(fouti, xs, i, Q);
		//foutx << "# x_" << i_OR + 1 <<" score vector:";
		//P.printOriginal(foutx, xs[i], "");
	}
	fouti.close();
	delete[] mergeTo;
	delete[] convergence;
	delete[] Q;
	delete[] xs;
	delete[] xsnext;
	delete[] vs;
	for (int i = 0; i < K; i++)
		delete[] keyP[i];
	delete[] keyP;
	delete[] hxs;
	delete[] shuffleIndex;
	cout << "Please check detected communities result in ./communities.dat." << endl;
}



/*
*The entrance of MQLC,
*/

int main_entrance(int argc, char** argv)
{
	// 1. The variables to store the input and output file names or folders.
	string sInputFileNameEdges = "./graph/com-amazon.ungraph.txt"; // Input file name for edges of the network.
	string sInputFileNameSeeds = "./graph/demo_seeds.txt"; // Input file name for the seed nodes.
	string sOutputFolder = "./"; // The folder to output the results.
	string sMapfile = "";
	string message = "";

	// 2. Parsing the input parameters.
	int nIdx = 1; // index for parsing the input parameters
	while (nIdx < argc){
		string sToken1 = argv[nIdx];
		string sToken2 = argv[nIdx + 1];
		nIdx = nIdx + 2;
		if (sToken1.compare("-i") == 0) { // Input file name for nodes of the networks
			sInputFileNameEdges = sToken2;
		}
		else if (sToken1.compare("-s") == 0) { // Input file name for seed nodes
			sInputFileNameSeeds = sToken2;
		}

		else if (sToken1.compare("-o") == 0) { // Input file name for outputfile
			sOutputFolder = sToken2;
		}
		else if (sToken1.compare("-a") == 0) { // parameter: aplha
			alpha = atof(sToken2.c_str());
		}

		else if (sToken1.compare("-b") == 0) {// parameter: beta
			beta = atof(sToken2.c_str());
		}

		else if (sToken1.compare("-g") == 0) {// parameter: gamma_W
			gamma_W = atof(sToken2.c_str());
		}

		else if (sToken1.compare("-k") == 0) {// parameter: K
			K = atoi(sToken2.c_str());
		}
		else if(sToken1.compare("-e") == 0) {// parameter: epsilon
			epsilon = atof(sToken2.c_str());
		}
		else if (sToken1.compare("-t") == 0) { // parameter: theta
			theta = atof(sToken2.c_str());
		}
	}
	//calculate MAXITE
	int  maxite2 = (int)(log(epsilon/2) / log(beta));
	//int  maxite2 = (int)(log(epsilon) / log(beta));
	MAXITE = MYmin(MAXITE, maxite2); //maximum number of iterations 

	MQLC(sInputFileNameEdges, sInputFileNameSeeds, sOutputFolder, alpha, beta, gamma_W, K, epsilon, theta);
	
	return 0;
}

int main(int argc,char ** argv) {//**************************
	if (argc==1|| string(argv[1]).compare("-h") == 0 || string(argv[1]).compare("-help") == 0) {
		//sample case
		cout << "\nEXAMPLE:\n" <<
			"./MRW -i ./graph/com-amazon.ungraph.txt -o ./ -s ./graph/demo_seeds.txt -a 0.1 -b 0.6 -k 5 -g 0.3\n" << endl;
		//-a 0.1 -b 0.6 -k 5 -g 0.3
		//-a 0.1 -b 0.4 -k 5 -g 0.3
		//-a 0.1 -b 0.6 -k 3 -g 0.2 
		//-a 0.2 -b 0.5 -k 5 -g 0.3
		cout << "Parameters:\n" <<
			"-i InputFileNameEdge\n" <<
			"-s InputFileNameSeeds\n" <<
			"-o OutputFolder: default: current folder\n" <<
			"-a alpha: default: 0.1\n" <<
			"-b beta: default: 0.6 \n" <<
			"-g gamma: default: 0.3\n" <<
			"-k K: default: 5\n" <<
			endl;
		cout << "Do you want to run a demo? [y] or [n]" << endl;
		char a;
		cin >> a;
		while (a != 'y' && a != 'n') {
			cout << "Please type in [y] or [n]" << endl;
			cin >> a;
		}
				
		if (a == 'y') {
			main_entrance(argc, argv);
		}
		else if (a == 'n') {
			cout << "\nPlease exit and run the program with command line!" << endl;
			cout << "\nEXAMPLE:\n" <<
				"./MRW -i ./graph/com-amazon.ungraph.txt -o ./ -s ./graph/demo_seeds.txt -a 0.1 -b 0.6 -k 5 -g 0.3\n" << endl;
		}

		system("pause");
	}
	else
		main_entrance(argc, argv);
}

