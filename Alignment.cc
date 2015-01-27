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

#include "Alignment.hh" // This will also include by includes Matrix.hh
#include "wrapper.hh"
#include "prototype.hh"

// Input parameter matobj is a reference to a Matrix object.
// Input parameter param is a reference to a struct scoretp variable with scoring parameters.

// The constructor initializes some member variables by calling public
// functions of Matrix from the object matobj.
// Then it allocates memory for each of the three char arrays,
// produces an optimal alignment by tracing through the matrices in the object matobj,
// and saves the alignment along with its score and length and editnum
// and subinsertlen in corresponding member variables.
// The alignment is represented by using three rows of characters (three char arrays).
// If the lengths of both sequences is zero, prints out an error message.

// Note that the length of an optimal alignment cannot exceed the sum of
// the lengths of the two DNA sequences.
Alignment::Alignment(Matrix &matobj, struct scoretp &param) :
		origin(matobj.getOrigin()), derived(matobj.getDerived())
{
	score = 0;
	editnum = 0;
	subinsertlen = 0;
	alignlen = (matobj.getRowInd() + matobj.getColInd());

	top = new char[alignlen];
	mid = new char[alignlen];
	bot = new char[alignlen];

	for (int x = 0; x < alignlen; x++)
		top[x] = ' ', mid[x] = ' ', bot[x] = ' ';

	int i = 0, j = 0;
	int len = 0;
	int intlen = 0;
	int m = origin.getLength();
	int n = derived.getLength();

	int mat = 'S';

	while (i <= m && j <= n)
	{
		if (mat == 'S')
		{
			if (i == m && j == n)
				break;
			if (j == n || ((matobj.getMat('S')[i][j]) == (matobj.getMat('D')[i][j])))
			{
				mat = 'D';
				editnum++;
				continue;
			}
			if (i == m || ((matobj.getMat('S')[i][j]) == (matobj.getMat('I')[i][j])))
			{
				mat = 'I';
				editnum++;
				intlen = 0;
				continue;
			}

			// append a_i+1 to TOP
			top[len] = origin.getSeq()[i + 1];

			// append match or mismatch character
			if (origin.getSeq()[i + 1] == derived.getSeq()[j + 1])
				mid[len] = '|';
			else
			{
				mid[len] = ' ';
				editnum++, subinsertlen++;
			}
			// append bot
			bot[len] = derived.getSeq()[j + 1];
			i++, j++, len++;
			continue;
		}
		if (mat == 'D')
		{
			top[len] = origin.getSeq()[i + 1];
			mid[len] = '-';
			bot[len] = ' ';

			if (i == m - 1
					|| ((matobj.getMat('D')[i][j]) == ((matobj.getMat('S')[i + 1][j]) - param.gext - param.gopen)))
			{
				mat = 'S';
			}
			i++;
			len++;
			continue;
		}
		if (mat == 'I')
		{
			top[len] = ' ';
			mid[len] = '-';
			bot[len] = derived.getSeq()[j + 1];
			intlen++;

			if (j == n - 1
					|| ((matobj.getMat('I')[i][j]) == ((matobj.getMat('S')[i][j + 1]) - param.gext - param.gopen)))
			{
				mat = 'S';
				subinsertlen += intlen;
			}
			j++;
			len++;
			continue;
		}
	}
	score = matobj.getMat('S')[0][0];
	alignlen = len;
}

// Frees heap memory.
Alignment::~Alignment() // destructor definition
{
	cout << "Freeing Alignments." << endl;

	delete[] top;
	delete[] mid;
	delete[] bot;
}

Direct &Alignment::getOrigin() const
// returns origin.
{
	return origin;
}

Direct &Alignment::getDerived() const
// returns derived.
{
	return derived;
}

int Alignment::getScore() const
// returns alignment score.
{
	return score;
}

int Alignment::getEditNum() const
// returns editnum.
{
	return editnum;
}

int Alignment::getSubInsertLen() const
// Returns subinsertlen.
{
	return subinsertlen;
}

int Alignment::getAlignLen() const
// Returns alignment length.
{
	return alignlen;
}

char *Alignment::getRow(char kind) const
// Returns top if kind is 'T'.
// Returns mid if kind is 'M'.
// Returns bot if kind is 'B'.
// Prints out an error if kind has no expeced value.
{
	switch (kind)
	{
	case 'T':
		return top;
		break;
	case 'M':
		return mid;
		break;
	case 'B':
		return bot;
		break;
	default:
		fatal("Unexpected value for getRow(char kind");
	}
	return NULL;
}

string Alignment::toString() const
// The function returns a summary of sequence and alignment information
// and the alignment in the form of string.
// The summary includes the name and length of each sequence,
// the score and length of the alignment. the number of edit operations,
// and the length of substitutions and insertions.
// The alignment is reported in sections of 70 characters, with each section consisting
// of three rows. The sequence positions of the first DNA letters in each section are
// reported in the left margin of 10 spaces.
{
	int total_count = 0;
	int A_count = 0;
	int B_count = 0;
	int origin_len = origin.getLength();
	int deriv_len = derived.getLength();

	std::ostringstream oss;
	oss << "Sequence A: " + origin.getName() << endl << "    Length: " << origin_len << endl
			<< "Sequence B: " + derived.getName() << endl << "    Length: " << deriv_len << endl << endl
			<< "Alignment Score: " << score << endl << "         Length: " << alignlen << endl
			<< "Number of Edit Operations: " << editnum << endl << "Length of Substitutions and Insertions: "
			<< subinsertlen << endl << endl;

	while (total_count < alignlen)
	{
		// Output marker line
		oss << setw(10) << (total_count + 1);
		// add a '.' for multiple of 5, add a ':' for multiples of 10.
		int end_count;
		if ((total_count + 70) > alignlen)
			end_count = alignlen - total_count;
		else
			end_count = 70;
		for (int i = 0; i <= end_count; i++)
		{
			// stop condition for meeting total length
			if (end_count > 0)
				if (i >= end_count)
					break;

			if (i % 10 == 5)
				oss << ".";
			else if (i % 10 == 0 && i > 5)
				oss << ":";
			else
				oss << " ";

		}
		oss << endl; // end marker line

		// output top sequence
		oss << setw(10) << (A_count + 1) << ' ';
		for (int j = total_count; j < total_count + 70; j++)
		{
			if (j == alignlen)
				break;

			oss << top[j];
			// only count letters, for index position
			if (top[j] != ' ')
				A_count++;
		}
		oss << endl;

		// output mid sequence
		oss << setw(12);
		for (int k = total_count; k < total_count + 70; k++)
		{
			if (k == alignlen)
				break;
			oss << mid[k];
		}
		oss << endl;

		oss << setw(10) << (B_count + 1) << " ";
		for (int l = total_count; l < total_count + 70; l++)
		{
			if (l == alignlen)
				break;

			oss << bot[l];
			// only count letters
			if (bot[l] != ' ')
				B_count++;
		}
		oss << endl << endl;
		total_count += 70;
		if (total_count > alignlen)
		{
			end_count = alignlen - total_count;
			total_count += end_count;
		}

	}
	string ret = oss.str();
	return ret;
}
