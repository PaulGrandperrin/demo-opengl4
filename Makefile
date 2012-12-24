CC = g++
CFLAGS=-Wall -Wextra -O0 -g -ggdb -p -pg -std=c++11 -pedantic -Wstrict-aliasing -DDEBUG
LDFLAGS= -Wl,--no-as-needed -lglfw -lGLEW -lIL -lSOIL

CC=clang++
CFLAGS= -O3 -g -ggdb -Wall -Wextra -pedantic -Wno-local-type-template-args -Wno-unused-parameter -Wno-logical-op-parentheses -std=c++11
LDFLAGS= -lglfw -lGLEW -lGL -lIL -lSOIL

all: demo

demo: main.o graphicEngine.o
	$(CC) $(LDFLAGS) main.o graphicEngine.o -o demo

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

graphicEngine.o: graphicEngine.cpp graphicEngine.hpp
	$(CC) $(CFLAGS) -c graphicEngine.cpp

clean:
	rm -f *.o demo

