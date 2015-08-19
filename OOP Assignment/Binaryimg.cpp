#include "header.h"

using namespace std;

BinaryImage::BinaryImage(int M, int N, double* input_data, double threshold)
{	
	for (int i = 0; i < M*N; i++) //Iterate though the array of size M*N
		{

			if (input_data[i] > threshold)  //If element value is greater than the threshold (170)
			{ 
				input_data[i] = 1;  //Set element value to 1 (white pixel)
			}
			else
			{
				input_data[i] = 0; //If value is less than threshold, set to 0 (black pixel)
			}
		}
}

BinaryImage::~BinaryImage() //BinaryImage destructor
{
	delete[] data;  //Delete data
	data = 0;
}

