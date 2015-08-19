#ifndef OBJECT_H
#define OBJECT_H
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <tchar.h>
#include <cmath>


using namespace std;

class Matrix
{
private:
	int M, N;
	
public:
	Matrix(){};
	double* data;

	double* readTXT(char *fileName, int sizeR, int sizeC);
	Matrix(int sizeR, int sizeC, double val); 
	Matrix(int sizeR, int sizeC, double* input_data);
	Matrix(const Matrix& m);
	~Matrix();

	Matrix getBlock(int stRow, int stCol, int enRow, int enCol) const;
	void setBlock(int stRow, int stCol, int enRow, int enCol, Matrix& m);
	double get(int i, int j) const{ return data[i*N + j]; }
	int getM() const{ return M; };
	int getN() const{ return N; };
	double getData(int i) { return data[i]; };
	void set(int i, int j, double val) { data[i*N + j] = val; };

	Matrix operator+ (const Matrix& m);
	Matrix operator- (const Matrix& m);
	Matrix operator* (const Matrix& m);
	Matrix operator/ (const Matrix& m);

	Matrix& operator= (const Matrix& m);
	Matrix operator++ ();

	void Print();

};

class BinaryImage : public Matrix
{
private:

public:
	BinaryImage(){};

	BinaryImage(int M, int N, double* input_data, double threshold);
	~BinaryImage();
};

#endif
