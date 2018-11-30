#pragma once
#ifndef _HEAD_MATRIX
#define _HEAD_MATRIX
#include<fstream>
#include"tools.h"
class Matrix {
public:
	Matrix();
	Matrix(int, int);
	void insert(int, int, float);
protected:
	int row, column;
};

class DenseMatrix : Matrix {
public:
	DenseMatrix(int, int);
	~DenseMatrix();
	void print();
	void insert(int, int, float);
	void ColumnNormalize();
	float rowSum(int);
	float columnSum(int);
	float getElement(int, int);
	void print(ofstream &fout);
	void deepCopyTo(DenseMatrix&);
	void deepCopyFrom(DenseMatrix&);
private:
	int size;
	float ** matrix;
};


#endif // !_HEAD_MATRIX
