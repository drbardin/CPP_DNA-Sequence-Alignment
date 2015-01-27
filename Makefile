# @author Dan Bardin

OBJS = main.o Alignment.o Direct.o Encoded.o Matrix.o wrapper.o
CC = g++
CFLAGS = -Wall -c -g -std=c++98
LFLAGS = -Wall -g

cmps : $(OBJS)
	$(CC)	$(LFLAGS)	$(OBJS)	-o	cmps

main.o : main.cc Alignment.hh Direct.hh Encoded.hh Matrix.hh wrapper.hh prototype.hh template.stack.min.hh
	$(CC)	$(CFLAGS)	main.cc

Alignment.o : Alignment.cc Alignment.hh wrapper.hh prototype.hh
	$(CC)	$(CFLAGS)	Alignment.cc
	
Direct.o : Direct.cc Direct.hh wrapper.hh prototype.hh
	$(CC)	$(CFLAGS)	Direct.cc
	
Encoded.o :	Encoded.cc Encoded.hh wrapper.hh prototype.hh
	$(CC)	$(CFLAGS)	Encoded.cc
	
Matrix.o : Matrix.cc Matrix.hh wrapper.hh prototype.hh
	$(CC)	$(CFLAGS)	Matrix.cc
	
wrapper.o : wrapper.cc wrapper.hh
	$(CC)	$(CFLAGS)	wrapper.cc
	
clean :
	\rm	*.o	cmps