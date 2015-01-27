/* @author Dan Bardin */

#ifndef WRAPPER_HH_
#define WRAPPER_HH_

#include <fstream>

void fatal(const char *msg); // a wrapper function
void fatal(const char *msg, const char *name); // a wrapper function
void ckopeninf(ifstream &infile, const char *fname); // a wrapper function

#endif /* WRAPPER_HH_ */
