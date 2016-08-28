#include "Vec3.hpp"

Vec3 Vec3::operator+(Vec3 v2) {
    return Vec3(x + v2.x, y + v2.y, z + v2.z);
}

void Vec3::operator+=(Vec3 v2) {
    x += v2.x;
    y += v2.y;
    z += v2.z;
}

Vec3 Vec3::operator*(double f) {
    return Vec3(x * f, y * f, z * f);
}

Vec3 Vec3::operator-(Vec3 v2) {
    return Vec3(x - v2.x, y - v2.y, z - v2.z);
}

Vec3 Vec3::cross(Vec3 v2) {
    return Vec3(
        y * v2.z - v2.y * z,
        z * v2.x - v2.z * x,
        x * v2.y - v2.x * y);
}

double Vec3::dot(Vec3 v2) {
    return x * v2.x + y * v2.y + z * v2.z;
}

double Vec3::length() {
    return sqrt(dot(*this));
}

Vec3 Vec3::normalize() {
    double len = this->length();
    return Vec3(x / len, y / len, z / len);
}

Vec3 Vec3::zero() {
    return Vec3(0.0, 0.0, 0.0);
}

Vec3 Vec3::up() {
    return Vec3(0.0, 0.0, 1.0);
}
