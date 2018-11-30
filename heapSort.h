//
// Created by nathan on 22/10/15.
//

#ifndef HEAPBUILDER_HEAP_H
#define HEAPBUILDER_HEAP_H
#include<stdio.h>

class Heap {

public:

	//constructors and deconstructor
	Heap();
	Heap(int);
	~Heap();


	void insert(int newNum,float value);
	void clear();
	int *getHeap();
	int *getSortedHeap();
	float*getSortedScore();
private:

	bool compareToParent(int node);
	int getParent(int node);
	int getLevel(int node);
	void swapWithParent(int node);
	bool bubbleUp(int node);
	bool bubbleDown(int node, int stoppingPoint);
	void heapSort();
	void swapNodes(int node1, int node2);

	int mCurrentPos;
	int heapsize;
	int *mHeap;
	float *values;
	int *mSortedHeap;
	float *sortedHeapValues;

};


#endif //HEAPBUILDER_HEAP_H