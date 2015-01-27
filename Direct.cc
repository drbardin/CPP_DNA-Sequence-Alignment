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
#include <ctype.h> 

#include "Direct.hh"
#include "wrapper.hh"
#include "prototype.hh"

// Input parameter fname is a pointer to a string for storing the name of a DNA sequence file.
// A Direct object is constructed to save the name,
// length and DNA letters of the sequence in the file.
// A proper amount of memory needs to be allocated for the seq member.

// This constructor opens the file whose name is pointed to by fname,
// reads the name, length and DNA letters of the sequence in the file,
// dynamically allocates a proper amount of memory for the seq member
// and saves the data into the member variables.
// The constructor prints out a proper error message if fname is NULL,
// no file exists with the given name.
Direct::Direct(const char *fname)
{
	// declare input stream
	ifstream inf;
	stringstream ss;
	int seq_count = 0;
	int name_count = 0;
	char ch;

	ckopeninf(inf, fname); // open the file, check for open error
	int pos = inf.tellg(); // remember sequence start position

	for (; inf.get(ch);)  // get name len, without >
	{
		if (ch == '\n')
			break;
		if (ch != '>')
			name_count++;
	}
	for (; inf.get(ch);)
		if (ch != '\n')
			seq_count++; // get length of input.

	length = seq_count; // set length variable for sequence.

	inf.clear(); // clear flags
	inf.seekg(pos + 1); // rewind to start of sequence.

	// get space for sequence
	seq = new char[seq_count + 2];

	int i = 0;
	while (inf.get(ch)) // Set the temp name
	{
		if (ch == '\n')
			break;
		if (ch != '>')
			ss << ch;
	}
	ss << '\0';

	i = 1;
	seq[0] = ' ';
	seq[length + 1] = '\0';
	while (inf.get(ch)) // set the sequence
	{
		if (ch != '\n')
			seq[i++] = ch;
	}

	ss >> name;
	inf.close();
}
// Input parameter stag is a reference to a string object as the name of a sequence.
// Input parameter slen is the length of the sequence.
// Input parameter sarr is a pointer to a char array with the sequence
// saved at index 1 through index slen.

// If slen is negative or sarr is NULL, prints out an error message with fatal.
// A copy of the string object is saved in the member name.
// The value slen is saved in the member length.
// Memory is allocated to the member seq and a copy of the sequence is saved in seq.
Direct::Direct(string &stag, int slen, char *sarr)
{
	// check for valid input first.
	if (slen < 0)
		fatal("Sequence length must be non-negative: \n");
	else if (sarr == NULL)
		fatal("No sequence passed to constructor: \n");

	name = string(stag);    // save the name
	length = slen;  // save the length of the sequence.

	seq = new char[slen + 1]; // make room for sequence characters

	strcpy(seq, sarr); // copy over sequence.
}

// A deep copy is made.
Direct::Direct(const Direct &obj)
{
	name = obj.name;
	length = obj.length;
	seq = obj.seq;
}

// Sets seq to NULL.
Direct::Direct()
{
	seq = '\0';
	length = 0;
}

Direct::~Direct()
// Frees heap memory if seq is not NULL.
{
	cout << "Freeing Direct." << endl;
	if (seq != NULL)
		delete[] seq;
}
// Returns the member name.
string Direct::getName() const
{
	return name;
}

// Returns the member length.
int Direct::getLength() const
{
	return length;
}

// Returns the member seq.
char *Direct::getSeq() const
{
	return seq;
}
