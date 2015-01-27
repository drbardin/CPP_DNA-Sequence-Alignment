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

#include "Encoded.hh"
#include "Direct.hh"
#include "Alignment.hh"
#include "wrapper.hh"
#include "prototype.hh"

// Input parameter obj is a reference to an Alignment object holding an optimal
// alignment along with sequence and other information.

// The constructor initializes some members by calling public functions
// of Alignment from the object obj. Among these members are
// origin, the reference to the original sequence,
// editnum, the size of the array operation, and
// subinsertlen, the size of the array subinsertion.
// the name and length of the derived sequence.
// The constructor allocates memory to operation and subinsertion.
// Then it constructs the two arrays operation and subinsertion
// by follwing the method in Figure 2 of the project description.
// The construction uses top & mid & bot, the three rows of the optimal alignment,
// which are provided by the public functions of Alignment from the object obj.
Encoded::Encoded(Alignment &obj) :
		origin(obj.getOrigin())
{
	subinsertlen = obj.getSubInsertLen();
	editnum = obj.getEditNum();
	origin = obj.getOrigin();
	dname = obj.getDerived().getName();
	dlength = obj.getDerived().getLength();

	operation = new struct Edit[editnum];
	subinsertion = new char[subinsertlen + 1];

	int i = 1;
	int j = 1;
	int opind = 0;
	int snind = 0;

	for (int ind = 0; ind < obj.getAlignLen();)
	{
		if (obj.getRow('M')[ind] == '|')
		{
			i++;
			j++;
			ind++;
			continue;
		}
		if (obj.getRow('M')[ind] == ' ') //substitution case
		{
			operation[opind].position = i;
			operation[opind++].indel = 0;
			subinsertion[snind++] = obj.getRow('B')[ind];
			i++;
			j++;
			ind++;
			continue;
		}
		int gaplen = 0;
		operation[opind].position = i;

		if (obj.getRow('B')[ind] == ' ') // deletion gap case
		{
			int x = ind;
			while (obj.getRow('B')[x++] == ' ')
				gaplen++;

			i += gaplen;
			operation[opind++].indel = (-1 * gaplen);
			ind += gaplen;
		}
		else // insertion gap case
		{
			int x = ind;
			while (obj.getRow('T')[x++] == ' ')
				gaplen++;

			j += gaplen;
			operation[opind++].indel = gaplen;
			for (; gaplen; gaplen--)
				subinsertion[snind++] = obj.getRow('B')[ind++];
		}
	}
	subinsertion[subinsertlen + 1] = '\0'; // null-term the string
}

Encoded::~Encoded()
// Frees heap memory.
{
	cout << "Freeing Encoded." << endl;

	delete[] operation;
	delete[] subinsertion;
}

char *Encoded::getDSeq() const
// Deirves a sequence of length dlength and returns it in a char array
// of length dlength + 2. The sequence is in the array from index 1
// to index dlength, with the element at index 0 set to ' '
// and that at index dlength + 1 to '\0'.
// The function allocates memory to the array and generates
// the sequence B by using the method in Figure 3 of the project description.
// The information on the sequence A is obtained by calling the public
// functions of Direct from the object reference origin.
{
	char *B = new char[dlength + 2];

	int i = 1, j = 1;
	int oppos, indel;
	int snind, opind;

	B[0] = ' '; // Empty 0 position
	B[dlength + 1] = '\0'; // null terminate string

	for (snind = 0, opind = 0; opind < editnum; opind++)
	{
		oppos = operation[opind].position;
		indel = operation[opind].indel;

		if (i < oppos)
		{
			for (int k = 0; k <= (oppos - 1 - i); k++)
				B[j + k] = origin.getSeq()[i + k];
			j += (oppos - i);
			i = oppos;
		}

		if (indel < 0)
			i -= indel;
		else
		{
			if (!indel)
				i++;
			B[j++] = subinsertion[snind++];

			for (; indel > 1; indel--)
				B[j++] = subinsertion[snind++];
		}
	}
	if (i <= origin.getLength())
	{
		for (int k = 0; k <= (origin.getLength() - i); k++)
		{
			B[j + k] = origin.getSeq()[i + k];
		}
	}

	return B;
}

Direct &Encoded::getOrigin() const
// Returns origin.
{
	return origin;
}

int Encoded::getEditNum() const
// Returns editnum.
{
	return editnum;
}

struct Edit *Encoded::getOperation() const
// Returns operation.
{
	return operation;
}

int Encoded::getSubInsertLen() const
// Returns subinsertlen.
{
	return subinsertlen;
}

char *Encoded::getSubInsertion() const
// Returns subinsertion.
{
	return subinsertion;
}

int Encoded::getDLength() const
// Returns dlength.
{
	return dlength;
}

string Encoded::getDName() const
// Returns dname.
{
	return dname;
}

string Encoded::toString() const
// Generates and returns a string form of its contents.
// The information includes the following lines:
// Name of the encoded sequence: ...
// Length of the encoded sequence: ...
// Number of edit operations: ...
// Length of substitutions and insertions: ...
// Concatenation of subs and inserts in order: ...
//
// and each of the operations in the array operation.
{
	ostringstream oss;
	stringstream ss;
	string ret;
	string sub_char_list;

	// read char array into string stream
	for (int x = 0; x < subinsertlen + 1; x++)
		ss << subinsertion[x];
	// send new string to concat list
	ss >> sub_char_list;

	// Output header information
	oss << "Name of the encoded sequence: " << getDName() << endl;
	oss << "Length of the encoded sequence: " << dlength << endl;
	oss << "Number of edit operations: " << editnum << endl;
	oss << "Length of substitutions and insertions: " << subinsertlen << endl;
	oss << "Concatenation of subs and inserts in order " << sub_char_list << endl;

	int subind = 0; // Index for char string.
	for (int i = 0; i < editnum; i++) // Loop through each edit.
	{
		// Output Insertion, Deletion, or Substitution based on subinsertion[n].indel
		oss << "Operation " << i << "::" << " Position: " << operation[i].position << " Indel: " << operation[i].indel;
		if (operation[i].indel > 0)
		{
			oss << " Insertion: ";
			for (int j = 0; j < operation[i].indel; j++)
				oss << subinsertion[subind++];
			oss << endl;
		}
		else if (operation[i].indel == 0)
			oss << " Substitution: " << subinsertion[subind++] << endl;
		else if (operation[i].indel < 0)
			oss << " Deletion" << endl;
	}

// turn output stringstream in string
	ret = oss.str();
	return ret;
}

bool Encoded::operator <=(Encoded &rightobj) const
{
	if( getNumDiff() == rightobj.getNumDiff())
		return getSubInsertLen() <= rightobj.getSubInsertLen();
	else
		return getNumDiff() <= rightobj.getNumDiff();

}

int Encoded::getNumDiff() const
{
	int num_subs = 0;
	int indel_count = 0;

	for(int i = 0; i < editnum; i++)
	{
		// substitution case, increment sub counter
		if(operation[i].indel == 0)
			num_subs++;
		else if(operation[i].indel < 0)// indel count = |indel|
			indel_count += (-1 * operation[i].indel);
		else
			indel_count += (operation[i].indel); // add number of edits
	}
	// return the total len of insert/del gaps and num of substitutions
	return num_subs + indel_count;
}
// create Compressed obj by calling superclass constructor via init list.
Compressed::Compressed(Alignment &obj) :
		Encoded(obj) {
}

bool Compressed::operator<=(Encoded &rightobj) const
{
	/*
	 *  strcmp(str1, str2);
	 * 	if ret < 0 => str1 < str2
	 * 	if ret > 0 => str2 < str1
	 * 	if ret = 0 => equal
	 */
	// if getNumDiff() for left and right are equal,
	// compare the strings of leftobj and rightobj.
	if(getNumDiff() == rightobj.getNumDiff())
	{
		return strcmp(getSubInsertion(), rightobj.getSubInsertion());
	}
	// otherwise, return the results of comparing getNumDiff()
	return getNumDiff() <= rightobj.getNumDiff();
}
