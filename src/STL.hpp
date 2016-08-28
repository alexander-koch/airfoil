#ifndef __STL_WRITER_HPP__
#define __STL_WRITER_HPP__

#include <vector>
#include <cstring>
#include <string>
#include "Vec3.hpp"
using std::string;
using std::vector;

struct __attribute__((packed)) BinarySTL {
    uint8_t header[80];
    uint32_t triangle_count;
};

struct __attribute__((packed)) STLTriangle {
    float normal[3];
    float v0[3];
    float v1[3];
    float v2[3];
    uint16_t attrib;
};

class STLWriter {
public:
    STLWriter();

    void setName(const string& name);
    void pushTriangle(Vec3 v0, Vec3 v1, Vec3 v2, Vec3 normal);
    unsigned getTriangleCount();
    void write(const string& filename, bool ascii);
private:
    string name;
    vector<STLTriangle> triangles;
};

#endif
