#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include "heapSort.h"
#include<cstring>
#include <time.h>
#include<iostream>

using namespace std;
Heap::Heap() {
	//set current pos equal to zero
	mCurrentPos = 0;
	heapsize = 0;
	mHeap = NULL;
	values = NULL;
	mSortedHeap = NULL;
	sortedHeapValues = NULL;
}


/*
* Initial with size 
*/

Heap::Heap(int size) {
	//set current pos equal to zero
	mCurrentPos = 0;
	heapsize = size;
	mHeap = new int[size];
	memset(mHeap, 0, sizeof(int)*size);
	values = new float[size];
	memset(values, 0, sizeof(float)*size);
}


void Heap::clear() {
	memset(mHeap, 0, sizeof(int)*heapsize);
	memset(values, 0, sizeof(float)*heapsize);
}

Heap::~Heap() {
	if (mHeap != NULL) {
		delete[] mHeap;
		mHeap = NULL;
	}

	if (values!=NULL) {
		delete[] values;
		values = NULL;
	}
}

void Heap::insert(int newNum,float value) {

	if (value <= values[0]) {
		return;
	}

	//insert a number into the current position
	mHeap[0] = newNum;
	values[0] = value;
	//bubble up the new entry
	//bubbleUp(mCurrentPos);
	bubbleDown(0,heapsize);
	//update currentpos
	//mCurrentPos++;

	//for (int i = 0; i < heapsize; i++)
	//	//cout << mHeap[i] << "\t" << values[i] << endl;
	//	cout << values[i] << " ";
	//cout << endl;

}


int *Heap::getHeap() {
	return mHeap;
}

int *Heap::getSortedHeap() {
	heapSort();
	return mHeap;
}

float * Heap::getSortedScore(){
	return values;
}

//return true if node is smaller than parent
bool Heap::compareToParent(int node) {
	return values[node] < values[getParent(node)] ? true : false;
}

int Heap::getParent(int node) {
	return (node - 1) / 2;
}

int Heap::getLevel(int node) {
	return 0;
}

void Heap::swapWithParent(int node) {

	swapNodes(node, getParent(node));

}

bool Heap::bubbleUp(int node) {

	//if at the root of the tree
	//break from this function
	if (getParent(node) < 0) {
		return false;
	}

	//if child is larger than parent then swap
	if (compareToParent(node)) {

		swapWithParent(node);
		//recursivly bubble until all is
		//well in the universe
		bubbleUp(getParent(node));

	}

	//bubble up has completed
	return false;
}


//this is going to be similar to the bubble up function but reverse obviously
bool Heap::bubbleDown(int node, int stoppingPoint) {

	//if we are at an array element already swapped then stop
	if (node >= stoppingPoint) {
		return false;
	}

	//if this is attempted on a index at the bottom or lower of the array
	//return false and stop at the bottom
	if (node >= heapsize - 1) {
		return false;
	}

	int leftChild, rightChild;

	//assuming counting from node 0
	leftChild = ((node + 1) * 2) - 1;
	rightChild = (node + 1) * 2;

	//last position
	if (rightChild == stoppingPoint) {

		if (values[leftChild] < values[node]) {
			swapNodes(node, leftChild);
		}
		return false;
	}

	//if we are at an array element already swapped then stop
	if (leftChild >= stoppingPoint) {
		return false;
	}




	//first check if right child is a thing
	if (rightChild < stoppingPoint) {

		//second check that parent isn't the smaller
		if (values[node] <= values[leftChild] &&
			values[node] <= values[rightChild]) {
			return false;
		}

		//if right child is smaller
		if (values[rightChild] < values[leftChild]) {
			swapNodes(node, rightChild);
			bubbleDown(rightChild, stoppingPoint);
			return false;

		}

		//if the left child is smaller
		if (values[leftChild] <= values[rightChild]) {
			swapNodes(node, leftChild);
			bubbleDown(leftChild, stoppingPoint);
			return false;
		}


	}

	//only need to check with left child
	if (leftChild <= stoppingPoint) {

		//if left child is larger than node
		if (values[leftChild] < values[node]) {
			swapNodes(node, leftChild);
			//no need to try any more here
			return false;
		}

	}
	return false;
}


//going to attempt this for first time
void Heap::heapSort() {



	for (int i = 0; i < heapsize; ++i) {
		//TODO can have 0 - 0 here for swaps
		swapNodes(0, ((heapsize - i) - 1));
		bubbleDown(0, (heapsize - i) - 1);

	}

}

/*
* Similar to swapParent function
* but is different in that it will take two node
* positions and swap them
*/
void Heap::swapNodes(int node1, int node2) {


	int temp;
	float v;

	//swap the nodes
	temp = mHeap[node1];
	mHeap[node1] = mHeap[node2];
	mHeap[node2] = temp;
	
	//swap the values
	v = values[node1];
	values[node1] = values[node2];
	values[node2] = v;
}