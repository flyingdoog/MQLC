#include"Matrix.h"
#include<fstream>

Matrix::Matrix() {
	row = 0;
	column = 0;
}

Matrix::Matrix(int r, int col) {
	row = r;
	column = col;
}

void Matrix::insert(int i, int j, float s) {
	;
}

/*
* Using Dense Matrix to store
*/
DenseMatrix::DenseMatrix(int r, int c):Matrix(r,c){
	matrix = new float*[r];
	for (int i = 0; i < r; i++) {
		matrix[i] = new float[c];
		memset(matrix[i], 0, sizeof(float)*c);
	}
}

DenseMatrix::~DenseMatrix() {
	if (matrix == NULL)
		return;
	for (int i = 0; i < row; i++) {
		if (matrix[i] != NULL) {
			delete [] matrix[i];
			matrix[i] = NULL;
		}
	}
	delete [] matrix;
	matrix = NULL;
}

void DenseMatrix::insert(int i, int j, float s) {
	matrix[i][j] = s;
}

/*
* will change the Matrix
* 
*/
void DenseMatrix::ColumnNormalize() {//***********
	for (int j = 0; j < column; j++) {
		float sum = 0;
		for (int i = 0; i < row; i++) {
			sum += matrix[i][j];
		}
		if (sum != 0) {
			for (int i = 0; i < row; i++) {
				matrix[i][j] /= sum;
			}
		}
	}
}

float DenseMatrix::rowSum(int i) {
	float sum =0;
	for (int j = 0; j < column; j++) {
		sum += matrix[i][j];
	}
	return sum;
}

float DenseMatrix::columnSum(int j) {
	double sum = 0;
	for (int i = 0; i < row; i++)
		sum += matrix[i][j];
	return sum;
}

/*
* Not Safe
*/
float DenseMatrix::getElement(int i, int j) {
	return matrix[i][j];
}

void DenseMatrix::print() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++)
			cout << matrix[i][j] << "\t";
		cout << endl;
	}
}

void DenseMatrix::print(ofstream &fout) {//*******
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++)
			fout << matrix[i][j] << "\t";
		fout << endl;
	}
}


void DenseMatrix::deepCopyTo(DenseMatrix& to) {//****************
	if (to.row != row || to.column!=column) {
		cout << "copy error deepCopyTo@Matrix" << endl;
		return;
	}
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			to.matrix[i][j] = matrix[i][j];
		}
	}

}
void DenseMatrix::deepCopyFrom(DenseMatrix& from) {
	if (from.row != row || from.column != column) {
		cout << "copy error deepCopyTo@Matrix" << endl;
		return;
	}
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			matrix[i][j] = from.matrix[i][j];
		}
	}
}