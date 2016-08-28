CC := g++
CFLAGS := -I. -Wall -std=c++11
FILES := src/main.cc src/Vec3.cc src/STL.cc

all:
	$(CC) $(CFLAGS) $(FILES) -o airfoil
