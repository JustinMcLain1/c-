#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

#ifndef H_PROG2
#define H_PROG2

// data files
#define FILE1 "./assignment02a.dat"
#define FILE2 "./assignment02b.dat"

#define ITEM_W 8 // no of spaces for each printed number

// to open two data files
void open_files(ifstream&is1, ifstream&is2){
	is1.open(FILE1);
	is2.open(FILE2);
	if (!is1.is_open())
	{
		cout << "Error in opening file " << FILE1 << "! Exiting..";
		exit(EXIT_FAILURE);
	}
	if (!is2.is_open())
	{
		cout << "Error in opening file " << FILE2 << "! Exiting..";
		exit(EXIT_FAILURE);
	}
}


// to read contents of two data files
void read_data(ifstream&is, vector<vector<int>>&m){
	for (int i = 0; i < m.size(); i++) // row loop
	{
		// loop from index 0 to number of columns -1
		for (int j = 0; j < m[i].size(); j++) // column loop
		{
			is >> m[i][j]; // read data into matrix
		}
	}
}

// to print contents of all matrices
void print_data(const vector <vector<int> >&m){

	cout << endl;
	// loop from index 0 to number of rows -1
	for (int i = 0; i < m.size(); i++) // row loop
	{
		// loop from index 0 to number of columns -1
		for (int j = 0; j < m[i].size(); j++) // column loop
		{
			cout << m[i][j]; // display numbers
			if (j < m[i].size() - 1) {
				for (int k = 0; k < ITEM_W; k++)
					cout << " ";
			}
		}
		cout << endl;
	}
}

// to generate a matrix by multiplying two input matrices
void gen_data(const vector<vector<int>>&A, const vector<vector<int>>&B, vector<vector<int>>&C)
{
	int sum;
	// loop over number of rows in m1
	for (int i = 0; i < A.size(); i++)
	{
		// loop over number of columns in m2
		for (int j = 0; j < B[0].size(); j++)
		{
			sum = 0;
			// loop over number of columns( or number of rows in m2) in m1
			for (int k = 0; k < A[0].size(); k++)
			{
				sum += A[i][k] * B[k][j];
			}

			C[i][j] = sum;
		}
	}
}

#endif
