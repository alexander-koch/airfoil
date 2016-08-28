#include "STL.hpp"

STLWriter::STLWriter() : name("Default") {}

void STLWriter::setName(const string& name) {
    this->name = name;
}

void STLWriter::pushTriangle(Vec3 v0, Vec3 v1, Vec3 v2, Vec3 normal) {
    struct STLTriangle triangle;
    triangle.normal[0] = normal.x;
    triangle.normal[1] = normal.y;
    triangle.normal[2] = normal.z;

    triangle.v0[0] = v0.x;
    triangle.v0[1] = v0.y;
    triangle.v0[2] = v0.z;

    triangle.v1[0] = v1.x;
    triangle.v1[1] = v1.y;
    triangle.v1[2] = v1.z;

    triangle.v2[0] = v2.x;
    triangle.v2[1] = v2.y;
    triangle.v2[2] = v2.z;
    triangle.attrib = 0;

    triangles.push_back(triangle);
}

unsigned STLWriter::getTriangleCount() {
    return triangles.size();
}

void STLWriter::write(const string& filename, bool ascii) {
    FILE* fp = fopen(filename.c_str(), "wb");
    if(!fp) return;

    if(ascii) {
        fprintf(fp, "solid %s\n", name.c_str());
        for(auto &tri : triangles) {
            fprintf(fp, "facet normal %f %f %f\n", tri.normal[0], tri.normal[1], tri.normal[2]);
            fprintf(fp, "outer loop\n");
            fprintf(fp, "vertex %f %f %f\n", tri.v0[0], tri.v0[1], tri.v0[2]);
            fprintf(fp, "vertex %f %f %f\n", tri.v1[0], tri.v1[1], tri.v1[2]);
            fprintf(fp, "vertex %f %f %f\n", tri.v2[0], tri.v2[1], tri.v2[2]);
            fprintf(fp, "endloop\n");
            fprintf(fp, "endfacet\n");
        }
        fprintf(fp, "endsolid %s\n", name.c_str());
    } else {
        // Write header
        const char* data = name.c_str();
        size_t sz = sizeof(data);

        struct BinarySTL info;
        std::memset(info.header, 0, sizeof(char) * 80);
        std::memcpy(info.header, data, sz);
        info.triangle_count = triangles.size();
        fwrite(&info, sizeof(BinarySTL), 1, fp);

        // Write triangles
        for(auto &tri : triangles) {
            fwrite(&tri, sizeof(STLTriangle), 1, fp);
        }
    }

    fclose(fp);
}
