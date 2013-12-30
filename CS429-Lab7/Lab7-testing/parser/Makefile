CXX=g++
CXXFLAGS=-g -c -O0 -Wall -DVERBOSE=0
OBJ= parser.o	\
     pdpaction.o\
     pdp.o\
     pdpstate.o\
     rwaction.o\
     rwgroup.o

parser: $(OBJ)
	$(CXX) -o parser $(OBJ)

parser.o: parser.cc pdpstate.h pdpaction.h Makefile
	$(CXX) $(CXXFLAGS) parser.cc

pdpaction.o: pdpaction.cc pdpaction.h pdpstate.h rwaction.h rwgroup.h parser_exception.h Makefile
	$(CXX) $(CXXFLAGS) pdpaction.cc

pdp.o: pdp.cc pdp.h Makefile
	$(CXX) $(CXXFLAGS) pdp.cc

pdpstate.o: pdpstate.cc pdpstate.h pdpaction.h Makefile
	$(CXX) $(CXXFLAGS) pdpstate.cc

rwaction.o: rwaction.cc rwaction.h Makefile
	$(CXX) $(CXXFLAGS) rwaction.cc

rwgroup.o: rwgroup.cc rwgroup.h Makefile
	$(CXX) $(CXXFLAGS) rwgroup.cc

test: parser
	./parser full.log f2.log

t: parser
	./parser f4.log f4.log

v: parser
	valgrind --log-file=l --trace-malloc=yes ./parser f4.log f4.log

t2: parser
	./parser fff.log fff.log

clean:
	rm -f $(OBJ) parser

g: parser
	gdb parser -x gt

