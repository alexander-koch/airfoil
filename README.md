# Airfoil

This project generates a 3D airplane wing based on the Kutta-Joukowski transformation.
A circle with a given radius r and two offsets xOffset and yOffset is mapped to
an airfoil by applying a coordinate of the circle z = x + iy to f(z).

```
f: C \ {0} ->  C
f(z) = 1/2 + (z + 1 / z)
```

The 3D Mesh data will be written in a ASCII / Binary STL file.

# Installation

To build the program, gcc is needed with support for c++11.
GNU Make should also be installed.

```
$ git clone [url]
$ cd airfoil
$ make
```

# License

MIT
