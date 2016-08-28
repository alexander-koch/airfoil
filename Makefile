CC := g++
CFLAGS := -I. -Wall -std=c++11
FILES := airfoil.cc Vec3.cc STL.cc

all:
	$(CC) $(CFLAGS) $(FILES) -o airfoil
