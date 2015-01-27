/* @author Dan Bardin */

#include <iostream> // provides objects like cin and cout for sending data
// to and from the standard streams input and output.
// These objects are part of the std namespace.
#include <cstdlib>  // has exit etc.
#include <fstream>  // file streams and operations
#include <sstream>  // string streams and operations
#include <iomanip>
using namespace std;

// a container for a set of identifiers.
// Because of this line, there is no need to use std::cout
#include <typeinfo> // This header file is included for using typeid.
#include <stdexcept>
#include <stdio.h>
#include <string.h>
#include <ctype.h>  // A library with functions for testing characters.

#include "Matrix.hh"
#include "wrapper.hh"
#include "prototype.hh"

// Input parameter seqone is a reference to a Direct object holding one DNA sequence.
// Input parameter seqtwo is a reference to a Direct object holding another DNA sequence.
// Input parameter param is a reference to a struct scoretp variable holding the scoring parameters.

// The constructor sets the member reference origin to seqone, and
// the member reference derived to seqtwo,
// through the initialization list before the constructor body.
//
// The constructor sets the member rowind to the return value from getLength() of seqone.
// The constructor sets the member colind to the return value from getLength() of seqtwo.
// If both rowind and colind are zero, then prints an error message.
// Then it allocates space for each
// matrix in the mattp structure by calling get2dspace(). Next it computes,
// in reverse order, the three matrices by the dynamic programming algorithm
// for aligning the two sequences pointed to by getLength() of seqone
// that of seqtwo. The sequence in seqone is treated as the sequence A.
Matrix::Matrix(Direct &seqone, Direct &seqtwo, struct scoretp &param) :
		origin(seqone), derived(seqtwo)  // an initialization list.
{
	rowind = seqone.getLength();
	colind = seqtwo.getLength();
	if (rowind == 0 && colind == 0)
		fatal("Sequence A and B contain no data. Terminating");

	Dmat = get2dspace(rowind + 1, colind + 1);
	Imat = get2dspace(rowind + 1, colind + 1);
	Smat = get2dspace(rowind + 1, colind + 1);

	int m = rowind;
	int n = colind;
	int i, j;

	Smat[m][n] = 0;
	Dmat[m][n] = Smat[m][n] - param.gopen;
	Imat[m][n] = Smat[m][n] - param.gopen;

	for (j = (n - 1); j >= 0; j--)
	{
		Imat[m][j] = (Imat[m][j + 1]) - param.gext;
		Smat[m][j] = (Imat[m][j]);
		Dmat[m][j] = (Smat[m][j]) - param.gopen;
	}

	for (i = (m - 1); i >= 0; i--)
	{
		Dmat[i][n] = (Dmat[i + 1][n]) - param.gext;
		Smat[i][n] = (Dmat[i][n]);
		Imat[i][n] = (Smat[i][n]) - param.gopen;

		for (j = (n - 1); j >= 0; j--)
		{
			// max 1 vals
			int D_one = (Dmat[i + 1][j]) - param.gext;
			int S_one = (Smat[i + 1][j]) - (param.gopen) - (param.gext);
			// Calculate max 1
			if (D_one > S_one)
				Dmat[i][j] = D_one;
			else
				Dmat[i][j] = S_one;

			//max 2 vals
			int I_one = (Imat[i][j + 1]) - param.gext;
			int S_two = (Smat[i][j + 1]) - (param.gopen) - (param.gext);
			// Calculate max 2
			if (I_one > S_two)
				Imat[i][j] = I_one;
			else
				Imat[i][j] = S_two;

			// get score of (a_i,b_j)
			int score;
			if (seqone.getSeq()[i + 1] != seqtwo.getSeq()[j + 1])
				score = param.mismat;
			else
				score = param.match;

			// find best score for S[i][j]
			int S_val = Smat[i + 1][j + 1] + score;
			int D_val = Dmat[i][j];
			int I_val = Imat[i][j];

			// short-circuit boolean, find max of 3 numbers.
			int num = S_val;
			(num < D_val) && (num = D_val);
			(num < I_val) && (num = I_val);
			Smat[i][j] = num;

		}
	}
}

// Frees heap memory.
Matrix::~Matrix()
{
	cout << "Freeing Matrices. " << endl;

	free2dspace(rowind + 1, Smat);
	free2dspace(rowind + 1, Imat);
	free2dspace(rowind + 1, Dmat);
}

// Allocates a 2-dimensional int array of (rowind + 1) by (colind + 1)
// and returns its memory address.
// Prints out an error message if rowind or colind is less than 0.
int **Matrix::get2dspace(int rowind, int colind)
{
	if (rowind < 0 || colind < 0)
		fatal("rowind or colind is less than 0");

	int **temp = new int *[rowind];
	for (int i = 0; i < rowind; i++)
		temp[i] = new int[colind];

	return temp;
}

void Matrix::free2dspace(int rowind, int **arr)
// Deallocates a 2-dimensional int array of first dimension
// (rowind + 1) by releasing the memory for arr[j] for each j
// and then releasing the memory for arr.
// Prints out an error message if rowind is less than 0.
{
	if (rowind < 0)
		fatal("Cannot free space < 0");

	for (int i = 0; i < rowind; i++)
		delete[] arr[i];
	delete[] arr;
}

Direct &Matrix::getOrigin() const
// Returns origin.
{
	return origin;
}

Direct &Matrix::getDerived() const
// Returns derived.
{
	return derived;
}

int Matrix::getRowInd() const
// Returns rowind.
{
	return rowind;
}

int Matrix::getColInd() const
// Returns colind.
{
	return colind;
}

int **Matrix::getMat(char kind) const
// If kind is 'S', then returns Smat.
// If kind is 'D', then returns Dmat.
// If kind is 'I', then returns Imat.
// Otherwise, prints out an error message.
{
	if (kind == 'S')
		return Smat;
	else if (kind == 'D')
		return Dmat;
	else if (kind == 'I')
		return Imat;
	else
	{
		fatal("Could not find matrix to return. \n");
		return NULL;
	}
}

string Matrix::toString(char kind) const
// Input parameter kind denotes matrix type: 'D', 'I', or 'S'.
// The function returns the matrix type and each value in the matrix
// in the form of a string.
// This function is for your own use to check on each matrix,
// so any format is OK.
{
	int **temp = new int *[colind + 1];
	for (int i = 0; i < colind + 1; i++)
		temp[i] = new int[rowind];

	temp = getMat(kind);

	cout << "Matrix Type: " << kind << endl;

	cout << " B          ";

	for (int i = 0; i < derived.getLength() + 1; i++)
		cout << derived.getSeq()[i] << "   ";
	cout << endl;
	cout << "Column   ";
	for (int j = 0; j < derived.getLength() + 1; j++)
		cout << "   " << j;
	cout << endl;
	cout << "A" << endl;

	for (int k = 0; k < origin.getLength() + 1; k++)
	{
		cout << std::setfill(' ') << origin.getSeq()[k] << " row " << std::setw(3) << k << " ";

		for (int l = 0; l < origin.getLength(); l++)
		{
			cout << std::setfill(' ') << std::setw(3) << temp[k][l] << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << endl;

	return string("End Matrix output \n");

}
