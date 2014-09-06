###
# Makefile
# Mateusz Szpakowski
###

.PHONY: all clean test

CXX = g++
AR = ar
RANLIB = ranlib
# CXXFLAGS = -Wall -std=gnu++11 -g
CXXFLAGS = -Wall -std=gnu++11 -O2 -funroll-loops
INCDIRS = -I.
LDFLAGS = -Wall
SOFLAGS = -fPIC

all: libNumStringConv.a libNumStringConv.so cstrtofXCStyle cstrtofXTest cstrtouXCStyle \
		fXtocstrCStyle

libNumStringConv.a: NumStringConv.o
	$(AR) cr $@ $^
	$(RANLIB) $@

libNumStringConv.so: NumStringConv.lo
	$(CXX) $(LDFLAGS) -shared -o $@ $^

cstrtofXCStyle: cstrtofXCStyle.o libNumStringConv.a
	$(CXX) $(LDFLAGS) -o $@ $^

cstrtofXTest: cstrtofXTest.o libNumStringConv.a
	$(CXX) $(LDFLAGS) -o $@ $^

cstrtouXCStyle: cstrtouXCStyle.o libNumStringConv.a
	$(CXX) $(LDFLAGS) -o $@ $^

fXtocstrCStyle: fXtocstrCStyle.o libNumStringConv.a
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c -o $@ $<

%.lo: %.cpp
	$(CXX) $(CXXFLAGS) $(INCDIRS) $(SOFLAGS) -c -o $@ $<

test: cstrtofXTest
	./cstrtofXTest

clean:
	rm -f *.o *.lo libNumStringConv.a libNumStringConv.so cstrtofXCStyle \
			cstrtofXTest cstrtouXCStyle fXtocstrCStyle
