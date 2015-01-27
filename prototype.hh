/* @author Dan Bardin */
#ifndef PROTOTYPE_HH
#define PROTOTYPE_HH

struct scoretp  // a structure for scoring parameters
{
	int match;  // a positive score for a pair of identical DNA letters
	int mismat; // a negative score for a pair of different DNA letters
	int gopen;  // a negative score for a gap
	int gext;   // a negative score for each letter in a gap
};

struct Edit        // a structure for an edit operation
{
	int position; // a sequence position to which the edit operation is applied
	short indel;    // indel < 0, the edit is a deletion gap of length -indel;
	// indel > 0, the edit is an insertion gap of length indel;
	// indel = 0, the edit is a substitution.
};

#endif /* PROTOTYPE_HH */