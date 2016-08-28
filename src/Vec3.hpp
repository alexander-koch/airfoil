#ifndef __VEC3_HPP__
#define __VEC3_HPP__

#include <cmath>

class Vec3 {
public:
    double x, y, z;

    Vec3(double nx, double ny, double nz) : x(nx), y(ny), z(nz) {}
    Vec3() : x(0), y(0), z(0) {}

    Vec3 operator+(Vec3 v2);
    void operator+=(Vec3 v2);
    Vec3 operator*(double f);
    Vec3 operator-(Vec3 v2);
    Vec3 cross(Vec3 v2);
    double dot(Vec3 v2);
    double length();
    Vec3 normalize();
    static Vec3 zero();
    static Vec3 up();
};

#endif
