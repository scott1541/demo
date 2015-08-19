#include "header.h"

using namespace std;


Matrix::Matrix(int sizeR, int sizeC, double val) //Overloaded constructor that accepts size and an array
{
	M = sizeR; //Set M and N values to sizes passed to constructor
	N = sizeC;

	data = new double[M*N]; //Reserve memory for new matrix

	for (int ii = 0; ii<M*N; ii++) //Iterate through matrix
	{
		data[ii] = val; //Set element of matrix to value from passed array
	}
}

Matrix::Matrix(int sizeR, int sizeC, double* input_data) //Overloaded constructor that accepts size values and pointer to an array
{
	M = sizeR;     //**Operation same as previous constructor above**
	N = sizeC;

	data = new double[M*N];

	for (int ii = 0; ii<M*N; ii++)
	{
		data[ii] = input_data[ii];
	}
}

Matrix::Matrix(const Matrix& m)  //Copy constructor
{
	M = m.getM(); //Get M and N values from copied matrix
	N = m.getN();
	data = new double[M*N];  //Reserve memory for new matrix, of size M*N
	for (int i = 0; i < M; i++) //Loops to iterate through copied matrix
		for (int j = 0; j < N; j++)
			data[i*N + j] = m.get(i, j); //Set element of matrix to value from element (i,j) of copied matrix

}

Matrix::~Matrix()  //Destructor
{
	delete[] data; //Delete unneeded data
	data = 0;

}

Matrix Matrix::getBlock(int stRow, int enRow, int stCol, int enCol) const //Get block
{
	int M = enRow - stRow + 1; //Using start and end rows/columns, calculate 
	int N = enCol - stCol + 1;
	double* block = new double[M * N]; //Create pointer to new double called block
	for (int j = 0; j < M * N; j++) //Iterate through values of 'block'
	{
		block[j] = data[(stRow * N + stCol) + j]; //Element of block is set to data of equivalent element in passed matrix
	}
		return Matrix(M, N, block); //Return matrix of size M * N, containing data from 'block'
} 

void Matrix::setBlock(int stRow, int enRow, int stCol, int enCol, Matrix& m) //Set block
{

	for (int i = 0; i < m.M; i++) //Nested loops to iterate though matrix
	{
		for (int j = 0; j < m.N; j++)
		{
			data[((stRow + i) * N) + (stCol + j)] = m.get(i, j);  //Set value to element in matrix to value of element in matrix m
		}
	}
}

Matrix Matrix::operator+(const Matrix& m) //Override + operator
{
	Matrix temp;                          //Overrides the addition operator to allow for the addition of two matrices

	temp.M = m.M;
	temp.N = m.N;

	temp.data = new double[temp.M*temp.N];
	
	for (int k = 0; k < (temp.M*temp.N); k++)
	{
		temp.data[k] = this->data[k] + m.data[k];  
	}

	return temp;
}

Matrix Matrix::operator-(const Matrix& m) //Override - operator
{
	Matrix temp;                          //Overrides subtraction operator to allow for the subtraction of two matrices

	temp.M = m.M;
	temp.N = m.N;

	temp.data = new double[temp.M*temp.N];

	for (int k = 0; k < (temp.M*temp.N); k++)
	{
		temp.data[k] = this->data[k] - m.data[k];
	}

	return temp;
}

Matrix Matrix::operator*(const Matrix& m) //Override * operator
{
	Matrix temp;                          //Overrides multiplication operator to allow for multiplication of matrices

	temp.M = m.M;
	temp.N = m.N;

	temp.data = new double[temp.M*temp.N];

	for (int k = 0; k < (temp.M*temp.N); k++)
	{
		temp.data[k] = this->data[k] * m.data[k];
	}

	return temp;
}

Matrix Matrix::operator/(const Matrix& m) //Override / operator
{
	Matrix temp;                          //Override division oprator to allow for division of matrices

	temp.M = m.M;
	temp.N = m.N;

	temp.data = new double[temp.M*temp.N];

	for (int k = 0; k < (temp.M*temp.N); k++)
	{
		temp.data[k] = this->data[k] / m.data[k];
	}

	return temp;
}

Matrix& Matrix::operator=(const Matrix& m) //Override = operator
{
	delete[] data;						   //Override equals operator to allow for values of one matrix to be assigned to another (copies data)

	M = m.M;
	N = m.N;

	data = new double[m.M*m.N];

	for (int k = 0; k < (m.M*m.N); k++)
	{
		data[k] = m.data[k];
	}
	return *this;
}

void Matrix::Print()  //Print function, outputs data in a matrix to console window
{
	for (int ii = 0; ii<M; ii++) //prints rows
	{
		for (int jj = 0; jj < N; jj++) //prints columns
		{
			cout << data[ii * N + jj] << " " << endl; //Uses cout to output elements of the matrix
		}
		cout << endl;
	}
}