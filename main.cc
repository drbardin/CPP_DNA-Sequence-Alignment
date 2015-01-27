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

#include "Alignment.hh"
#include "Direct.hh"
#include "Encoded.hh"
#include "Matrix.hh"
#include "template.stack.min.hh"
#include "wrapper.hh"
#include "prototype.hh"

int main(int argc, char *argv[])
{
	// Require at least one input file. 
	if (argc < 2)
	{
		cerr << "Usage: " << argv[0] << " Seq1, Seq2, ..., SeqN " << endl << endl;
		cerr << "Seq<n>        file of one sequence in FASTA format" << endl;
		exit(1);
	}

	// argv[1] - creates a Direct called 'origin' (full scope)
	Direct *origin = new Direct(argv[1]);
	
	// Create two stacks of type Stack<Encoded> (full scope)
	Stack<Encoded> e_stack;
	Stack<Encoded> c_stack;

	// create score struct & init members.
	struct scoretp *pScore = new struct scoretp;
	pScore->match = 10;
	pScore->mismat = -20;
	pScore->gopen = 40;
	pScore->gext = 2;

	// argv[2] -> argv[n] create a Direct object called 'derived' (block scope)
	// origin and derived create a Matrix object 'matobj' (block scope)
	// matobj used to create an Alignment object 'alignobj' (block scope)
	// alginobj used to create an Encoded object 'encodeobj' with new operator
	// encodeobj is pushed onto e_stack
	// alignobj also used to create Compressed object 'compobj'
	// compobj is pushed onto c_stack
	for(int i = 2; i < argc; i++){

		Direct *derived = Direct(argv[i]);
		Matrix *matobj = Matrix(origin, derived, pScore);
		Alignment *alignobj =  Alignment(matobj, pScore);
		Encoded *encodeobj = new Encoded(alignobj);
		Compressed *compobj = new Compressed(alignobj);

		e_stack.push(encodeobj);
		c_stack.push(compobj);
	}
	
	// find min and max for Encoded objects
	Encoded *e_min = findMin<Encoded>(e_stack);
	Encoded *e_max = findMax<Encoded>(e_stack);

	// find min and max for Compressed objects -> upcast to Encoded when placed on stack
	Encoded *c_min = findMin<Encoded>(c_stack);
	Encoded *c_max = findMax<Encoded>(c_stack);
	
	cout << "A stack of Encoded pointers" << endl;
	cout << "Min's number of differences: " << e_min->getNumDiff() << endl;
	cout << "Min's name: " << e_min->getDName() << endl;
	cout << "Min's derived sequence:" << endl;
	cout << e_min->getDSeq() << endl << endl;

	cout << "Max's number of differences: " << e_max->getNumDiff() << endl;
	cout << "Max's name: " << e_max->getDName() << endl;
	cout << "Max's derived sequence:" << endl;
	cout << e_max->getDSeq() << endl << endl;

	cout << "A stack of Compressed pointers" << endl;
	cout << "Min's number of differences: " << c_min->getNumDiff() << endl;
	cout << "Min's name: " << c_min->getDName() << endl;
	cout << "Min's derived sequence:" << endl;
	cout << c_min->getDSeq() << endl << endl;

	cout << "Max's number of differences: " << c_max->getNumDiff() << endl;
	cout << "Max's name: " << c_max->getDName() << endl;
	cout << "Max's derived sequence:" << endl;
	cout << c_max->getDSeq() << endl << endl;
	// Sequence encoded in the smallest object is obtained by calling 'getDseq()' 
	// from the object and sent to cout along with the values reported by funcs
	// getNumDiff() and getSubInsertLen(). 
	//  *** Steps are repeated for largest object. 

// sends each matrix to cout by calling the toString() of Matrix from matobj.
//  matobj->toString('S');
//  matobj->toString('D');
//  matobj->toString('I');

//	delete pararec;
//	delete seqone;
//	delete seqtwo;
//	delete[] dseq;
//	delete matobj;
//	delete alignobj;
//	delete encodeobj;

	return 0;
}
