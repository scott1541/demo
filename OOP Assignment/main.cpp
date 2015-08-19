#include "header.h"

using namespace std;

double* Task1(Matrix Mat1, Matrix Mat2); //Task 1 function definiton
double* Task2(Matrix Mat1, Matrix Mat2); //Task 2
double SSD(Matrix Mat1, Matrix Mat2);  //Function to calculate the SSD of two matrices
double* readTXT(char *fileName, int sizeR, int sizeC);  //Read data from text file to array of size R*C
void WritePGM(char *filename, double *data, int sizeR, int sizeC, int Q); //Output PGM file from passed array


int main()
{
	int opt;

	do  //Loop to select which task is performed, either 1 or 2
	{   //Error message upon invalid option selection
		opt = 0;
		cout << "What task would you like to perform?" << endl;
		cout << "\tTask 1" << endl << "\tTask 2 " << " (Please enter 1 or 2)" << endl; //Output to user 
		cin >> opt;

		if (opt == 1)
		{
			cout << "Task 1 running..." << endl;

			int M = 512;  //Dimensions for both images
			int N = 512;
			int Q = 1;  // Output file type, binary (1), or greyscale (255)
			double threshold = 170; //Threshold value for conversion to binary image
			char* fileName = "Files\\logo_with_noise.txt";  //File for logo with noise
			char* fileName2 = "Files\\logo_shuffled.txt";   //Shuffled logo
			char* outfilename = "Files\\restored_logo.pgm"; //Output file for restored logo

			double* input_data = readTXT(fileName, M, N);   //Input text from files to arrays
			double* input_data2 = readTXT(fileName2, M, N);

			BinaryImage Bi1(M, N, input_data, threshold);  //Convert noisy image to binary image
			BinaryImage Bi2(M, N, input_data2, threshold); //Convert shuffled image to binary

			Matrix Mat1(M, N, input_data); //unshuffled
			Matrix Mat2(M, N, input_data2); //shuffled
			
			delete[] input_data;  //Delete unneeded data
			delete[] input_data2;
			input_data = 0;
			input_data2 = 0;

			WritePGM(outfilename, Task1(Mat1, Mat2), M, N, Q);  //Write PGM file, using returned array, dimensions and file type

			break; //Break out of loop



		}

		if (opt == 2)
		{
			cout << "Task 2 running..." << endl;

			int MS = 768;  //Dimensions for scene
			int NS = 1024; //
			int MW = 48;   //Dimensions for wally, cut down from 49 to fit scene size
			int NW = 32;   //cut down from 36 to fit scene
			int Q = 255;   // Output file type, binary (1), or greyscale (255)
			char* fileName = "Files\\cluttered_scene.txt";  //Input scene 
			char* fileName2 = "Files\\wally_grey.txt";  //Input 'wally'
			char* outfilename = "Files\\wally_found.pgm"; //Ouput scene with wally found

			double* input_data = readTXT(fileName, MS, NS);   //Import scene file to array
			double* input_data2 = readTXT(fileName2, MW, NW); //Import wally file to array

			Matrix Mat1(MS, NS, input_data);  //Create matrices from inputted arrays
			Matrix Mat2(MW, NW, input_data2);
			
			delete[] input_data; //Delete unneeded data
			delete[] input_data2;
			input_data = 0;
			input_data2 = 0;

			WritePGM(outfilename, Task2(Mat1, Mat2), MS, NS, Q); //Write PGM file, using returned array, dimensions and file type

			break; //Break out of loop
		}
		else
			cout << "Invalid option!" << endl;
	} 
	while ( opt != 1 && opt != 2); 

	system("Pause"); 
	return 0;
}
double* Task1(Matrix Mat1, Matrix Mat2)  //Task 1
{
	int M = Mat2.getM(); //Get M and N values
	int N = Mat2.getN();
	int x, y, yy, xx, LoXX, LoYY;    //Declare variables to be used in for loops

	double SSDS;
	Matrix suMat1;  //Create sub-matrices for blocks of both images
	Matrix suMat2;
	Matrix Mat3(512, 512, 1.0); //Create new matrix for final output, with white background to show placed blocks


	
	for (x = 0; x <= 480; x += 32) //Iterate through matrix 1
	{	for (y = 0; y <= 480; y += 32)  
		{

			suMat1 = Mat1.getBlock(x, (x + 31), y, (y + 31)); //Set sub-matrix data to retrieved 32x32 block of matrix 1

			for (xx = 0; xx <= 480; xx += 32) //Iterate through matrix 2
			{	for ( yy = 0; yy <= 480; yy += 32)
			{
					
				double LoSSDS = 1000000; //Set high value so everything will be lower

					suMat2 = Mat2.getBlock(xx, (xx + 31), yy, (yy + 31));  //shuffled

					SSDS = SSD(suMat1, suMat2); // Calculate sum of squared difference (SSD)

					if (SSDS < LoSSDS)  //If SSD score is lower than current lowest
					{
						LoSSDS = SSDS;  //Set lowest to current SSD score
						LoXX = xx;      //Store location of lowest SSD block
						LoYY = yy;
						 
					}

				}
			}

			Mat3.setBlock(LoXX, (LoXX + 31), LoYY, (LoYY + 31), suMat2); //Set location of lowest SSD block to sub-matrix
																		 // from same location in opposing matrix
			
			cout << "Finished Block: " << x << ", " << y << endl; //Output message on progress

		}
	
	}


	delete[] suMat1.data; //Delete unneeded data
	suMat1.data = 0;
	delete[] suMat2.data;
	suMat2.data = 0;

	double* output_data = new double[M * N]; //Create new array to contain all data in matrix 2
	
	for (int i = 0; i < M; i++) //Iterate through matrix 2
	{	for (int j = 0; j < N; j++)
		{
			output_data[i*N + j] = Mat3.get(i, j);  //Set value in 1D array to the equvilent value in matrix 2
		}
	}
	
	delete[] Mat1.data; //Delete unneeded data
	Mat1.data = 0;
	delete[] Mat2.data; 
	Mat2.data = 0;

	return output_data;  //return array containing unshuffled image data
}

double* Task2(Matrix Mat1, Matrix Mat2)
{
	Matrix Mat3(32, 48, 255.0); //Create matrix the same size as matrix 2, but containing only white values (greyscale value 0)
	int x, y, LoXX, LoYY;     //Declaring variables 
	int M = Mat2.getM();      //Get dimensions
	int N = Mat2.getN();
	double LoSSDS = 100000;  //Set high value so every SSDS value will be lower
	double SSDS;

	for (x = 0; x <= 992; x += 32) //Iterate through matrix 1 in increments the same as matrix 2 dimensions
	{	for (y = 0; y <= 720; y += 48) 
		{
			Matrix suMat1; //Create sub-matrix

			suMat1 = Mat1.getBlock(x, (x + 31), y, (y + 47)); //Set sub-matrix with data from matrix 1

			SSDS = SSD(suMat1, Mat2);  //Calculate sum of squared difference

			delete[] suMat1.data;  //Delete unneeded data
			suMat1.data = 0;
			delete[] Mat2.data;
			Mat2.data = 0;

			if (SSDS < LoSSDS)  //If sum of squared difference is lower than current lowest SSD
			{
				LoSSDS = SSDS;  //Set lowest SSD to current SSD
				LoXX = x;       //Store location of block with lowest SSD
				LoYY = y;
			}

			cout << "Finished Block: " << x << ", " << y << endl; //Output progress
		}
 
	}

	Mat1.setBlock(LoXX, (LoXX + 31), LoYY, (LoYY + 47), Mat3); //Set location of lowest SSD block to matrix containing black values

	double* output_data = new double[M * N];

	for (int i = 0; i < M; i++)  //iterate through Matrix 1
	{	for (int j = 0; j < N; j++)
		{
			output_data[i*N + j] = Mat1.get(i, j);  //Set value in 1D array to the equvilent value in matrix 1
		}
	}

	delete[] Mat1.data; //Delete data
	Mat1.data = 0;

	return output_data; //Return array containing data from matrix 1 (with wally location marked)
}

double* readTXT(char *fileName, int sizeR, int sizeC)  //Get data from txt file
{
	double* input_data = new double[sizeR*sizeC];
	int i = 0;
	ifstream currentFile(fileName);
	if (currentFile.is_open())
	{
		while (currentFile.good())
		{
			if (i>sizeR*sizeC - 1) break;
			currentFile >> *(input_data + i);
			i++;


		}
		currentFile.close();
	}
	else
	{
		cout << "File path not found" << endl;
	}

	return input_data;

}

void WritePGM(char *filename, double *data, int sizeR, int sizeC, int Q) //Write PGM file
{

	int i;
	unsigned char *image;
	ofstream myfile;

	image = (unsigned char *) new unsigned char[sizeR*sizeC];

	// convert the integer values to unsigned char

	for (i = 0; i<sizeR*sizeC; i++)
		image[i] = (unsigned char)data[i];

	myfile.open(filename, ios::out | ios::binary | ios::trunc);

	if (!myfile) {
		cout << "Can't open file: " << filename << endl;
		exit(1);
	}

	myfile << "P5" << endl;
	myfile << sizeC << " " << sizeR << endl;
	myfile << Q << endl;

	myfile.write(reinterpret_cast<char *>(image), (sizeR*sizeC)*sizeof(unsigned char));

	if (myfile.fail()) {
		cout << "Can't write image " << filename << endl;
		exit(0);
	}

	myfile.close();

	delete[] image;

}

double SSD(Matrix Mat1, Matrix Mat2)   //Calculate squared difference
{
	double sumsd = 0;
	int M = Mat2.getM(); //Get dimensions, set to local variables
	int N = Mat2.getN();

	Matrix Difference = Mat1 - Mat2;  //Difference of matrices, one matrix minus the other
	
	delete[] Mat1.data; //Delete data in matrices that are no longer required
	delete[] Mat2.data;
	Mat1.data = 0;
	Mat2.data = 0;

	Matrix SSD = Difference * Difference; //Square the difference matrix to get 

	delete[] Difference.data; //Delete data in Difference matrix to save memory
	Difference.data = 0;

	for (int i = 0; i < (N-1); i++) //iterate through rows and columns of matrix
	{
		for (int j = 0; j < (M-1); j++)
		{
			sumsd += SSD.get(i, j);  //Add squared difference of element to sum of squared diff
		} 
	}
	
	delete[] SSD.data; //Delete data in SSD matrix
	SSD.data = 0;

	return sumsd;
}

