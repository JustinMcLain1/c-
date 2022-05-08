/* 	Justin McLain
	Z1910087
	CSCI 340- PE1
	
	I certify that this is my own work and where appropriate an extension 
	of the starter code provided for the assignment.
*/
#include "assignment02.h"

/*
   This program opens two data files for reading. If either file can't
   be opened, it prints an error message on stderr and aborts the
   program. It generates two input matrices from the data in data
   files, then generates a new matrix by multiplying the numbers in
   two input matrices. Finally, it prints all three matrices on
   stdout.
*/

int main ( )
{
    ifstream is1, is2;          // input file streams
    open_files(is1, is2);    // open input files

    // number of rows and colums in matrices A, B and C
    unsigned nrowsA , ncolsA , nrowsB  , ncolsB , nrowsC, ncolsC;

    // read number of rows and colums for input matrices from data
    // files and assign number of rows and colums to resulting
    // matrix from values of input matrices; first two values in
    // data file are number rows and colums of a matrix
    is1 >> nrowsA >> ncolsA; 
    is2 >> nrowsB >> ncolsB;
    nrowsC = nrowsA; 
    ncolsC = ncolsB;

    // define matrices as two-dimensional vectors
    vector<vector<int> > A(nrowsA, vector<int>(ncolsA));
    vector<vector<int> > B(nrowsB, vector<int>(ncolsB));
    vector<vector<int> > C(nrowsC, vector<int>(ncolsC));
    
    // read input values from data files
    read_data(is1, A); 
    read_data(is2, B);

    // generate values for matrix C by multiplying values in
    // matrices A and B, where C = AB
    gen_data(A, B, C);

    // print contents of all matrices on stdout
    cout << "\nMatrix1: "; print_data(A);
    cout << "\nMatrix2: "; print_data(B);
    cout << "\nMatrix3: "; print_data(C);
    cout << endl;

    return 0;
}
