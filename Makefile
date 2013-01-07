CXX=clang++
CXXFLAGS= -O3 -g -ggdb -Wall -Wextra -pedantic -Wno-local-type-template-args -Wno-unused-parameter -Wno-logical-op-parentheses -std=c++11
LDFLAGS= -lglfw -lGLEW -lGL -lIL -lSOIL

CXX = g++
CXXFLAGS=-Wall -Wextra -Wno-unused-parameter -O0 -g -ggdb -p -pg -std=c++11 -pedantic -Wstrict-aliasing -DDEBUG
LDFLAGS= -Wl,--no-as-needed -lglfw -lGLEW -lIL -lSOIL

CXX = g++
CXXFLAGS=-Wall -Wextra -Wno-unused-parameter -O3 -std=c++11 -pedantic -Wstrict-aliasing
LDFLAGS= -Wl,--no-as-needed -lglfw -lGLEW -lIL -lSOIL

all: demo



demo: main.o libDSGE.so
	$(CXX) $(LDFLAGS) main.o -L. -lDSGE -o demo

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

libDSGE.so: graphicEngine.o
	$(CXX) $(LDFLAGS) -shared -Wl,-soname,libDSGE.so graphicEngine.o -o libDSGE.so

graphicEngine.o: graphicEngine.cpp graphicEngine.hpp
	$(CXX) $(CXXFLAGS) -fPIC -c graphicEngine.cpp

clean:
	rm -f *.o *.so demo

