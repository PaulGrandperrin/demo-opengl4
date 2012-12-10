HARDENING_CFLAGS=-fPIE -fstack-protector -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security -fPIC
HARDENING_LDFLAGS=-fPIE -pie -Wl,-z,relro -Wl,-z,now

HARDENING_GCC_ONLY_CFLAGS=--param ssp-buffer-size=4
HARDENING_GCC_ONLY_LDFLAGS=



CC=clang
CFLAGS= -O3 -Wall -Wextra -std=c99 -pedantic
LDFLAGS= -O4 -Xlinker -plugin=LLVMgold.so -fPIE -pie -Wl,-z,relro -Wl,-z,now


CC = gcc
CFLAGS= -flto -Wall -Wextra -O3 -std=c99 -pedantic $(HARDENING_CFLAGS) $(HARDENING_GCC_ONLY_LDFLAGS)
LDFLAGS= -flto $(HARDENING_LDFLAGS) $(HARDENING_GCC_ONLY_LDFLAGS)

CC=clang++
CFLAGS= -O3 -g -ggdb -Wall -Wextra -pedantic -Wno-local-type-template-args -Wno-unused-parameter -Wno-logical-op-parentheses -std=c++11
LDFLAGS= -lglfw -lGLEW -lGL

CC = g++
CFLAGS=-Wall -Wextra -O0 -g -ggdb -p -pg -std=c++11 -pedantic -Wstrict-aliasing -DDEBUG
LDFLAGS= -Wl,--no-as-needed -lglfw -lGLEW

all: demo

demo: main.o graphicEngine.o
	$(CC) $(LDFLAGS) main.o graphicEngine.o -o demo

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

graphicEngine.o: graphicEngine.cpp graphicEngine.hpp
	$(CC) $(CFLAGS) -c graphicEngine.cpp

clean:
	rm -f *.o demo

