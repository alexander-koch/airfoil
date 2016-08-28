#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstdbool>
#include <algorithm>
#include <complex>
#include <vector>

#include "Vec3.hpp"
#include "STL.hpp"
#include "ObjectPool.hpp"

double xOffset;
double yOffset;
double radius;
double rad2;
double step_size = 0.01;
double sigma;
std::complex<double> I(0.0, 1.0);

std::complex<double> cexp(double alpha) {
    return cos(alpha) + I * sin(alpha);
}

class Index {
public:
    Vec3 position;
    Vec3 transformed;
    Vec3 normal;
    Index() {}
    Index(Vec3 pos, Vec3 trans, Vec3 n) : position(pos), transformed(trans), normal(n) {}
};

Index getTransform(double x, bool up) {
    double x2 = pow((x - xOffset), 2);
    double y = yOffset + sqrtf(rad2 - x2) * (up ? 1.0 : -1.0);
    std::complex<double> z1 = x + y * I;
    std::complex<double> z2 = 0.5 * (z1 + 1.0 / z1);
    return Index(Vec3(x, y, 0), Vec3(std::real(z2), 0.0, std::imag(z2)), Vec3::zero());
}

bool inTriangle(Vec3 a, Vec3 b, Vec3 c, Vec3 p) {
    Vec3 v0 = b - a, v1 = c - a, v2 = p - a;
    double d00 = v0.dot(v0);
    double d01 = v0.dot(v1);
    double d11 = v1.dot(v1);
    double d20 = v2.dot(v0);
    double d21 = v2.dot(v1);
    double denom = d00 * d11 - d01 * d01;

    double v = (d11 * d20 - d01 * d21) / denom;
    double w = (d00 * d21 - d01 * d20) / denom;
    double u = 1.0 - v - w;
    return v > 0 && w > 0 && u > 0;
}

double normal_pdf(double x, double m, double s) {
    static const double inv_sqrt_2pi = 0.3989422804014327;
    double a = (x - m) / s;
    return inv_sqrt_2pi / s * std::exp(-0.5f * a * a);
}

double get_step_size(double x, double sigma) {
    return std::max(std::min(normal_pdf(x, xOffset, sigma), 0.2), 0.02);
}

// Example: airfoil Wing 1.23 0.2 0.128 0.3
// High sigma: low poly
// Low sigma: high poly
int main(int argc, char** argv) {
    if(argc != 6) {
        std::cout << "Usage:\n  airfoil <wing_name> <radius> <xOffset> <yOffset> <sigma>" << std::endl;
        return 0;
    }

    // Fetch arguments
    radius = atof(argv[2]);
    xOffset = atof(argv[3]);
    yOffset = atof(argv[4]);
    sigma = atof(argv[5]);
    rad2 = radius * radius;

    // Define points
    std::vector<Index> points;
    Vec3 m = Vec3(0.0, 5.0, 0.0);

    // Iterate and transform circle
    double x = -radius + xOffset;
    step_size = get_step_size(x, sigma);
    for(; x <= radius+xOffset; x+=step_size) {
        step_size = get_step_size(x, sigma);
        points.push_back(getTransform(x, true));
    }

    step_size = get_step_size(x, sigma);
    for(x = radius+xOffset; x > -radius+xOffset; x-=step_size) {
        step_size = get_step_size(x, sigma);
        points.push_back(getTransform(x, false));
    }

    // Get chord length
    Vec3 min = points[0].transformed;
    Vec3 max = min;
    for(unsigned i = 0; i < points.size(); i++) {
        Vec3 current = points[i].transformed;
        if(current.x > max.x) max = current;
    }
    double c = (max - min).length();
    std::cout << "Chord length is: " << c << std::endl;
    std::cout << "Total points: " << points.size() << std::endl;

    STLWriter writer;
    writer.setName(argv[1]);
    for(unsigned i = 0; i < points.size(); i++) {
        int next_index = (i == points.size()-1) ? 0 : i+1;
        Index i1 = points[i];
        Index i2 = points[next_index];

        Vec3 v1 = i1.position;
        Vec3 v2 = i2.position;
        Vec3 v = v2 - v1;
        Vec3 n = Vec3::up().cross(v).normalize();
        i1.normal = n;

        Vec3 t1 = i1.transformed - m;
        Vec3 t2 = i2.transformed - m;

        Vec3 t1m = t1 + m * 2;
        Vec3 t2m = t2 + m * 2;

        // Connect wing faces
        n = (t2 - t1).cross(t1m - t1).normalize() * -1;
        writer.pushTriangle(t2, t1m, t1, n);

        n = (t2 - t2m).cross(t1m - t2m).normalize();
        writer.pushTriangle(t2, t2m, t1m, n);
    }

    ObjectPool<Index> pool(points);
    pool.buildCircular();

    Node<Index>* currentNode = pool.getFront();
    while(currentNode->prev != currentNode->next) {
        currentNode = currentNode->next;

        Node<Index>* prevNode = currentNode->prev;
        Node<Index>* nextNode = currentNode->next;
        Index i0 = prevNode->value;
        Index i1 = currentNode->value;
        Index i2 = nextNode->value;

        Vec3 prev = i0.transformed;
        Vec3 cur = i1.transformed;
        Vec3 next = i2.transformed;

        Vec3 a = prev-cur;
        Vec3 b = next-cur;
        double wedgeProd = (a.x * b.z) - (a.z * b.x);
        if(wedgeProd <= 0) continue;

        bool overlap = false;
        for(auto &point : points) {
            if(inTriangle(cur, next, prev, point.transformed)) {
                overlap = true;
                break;
            }
        }
        if(overlap) continue;

        // Build triangles faces
        writer.pushTriangle(next - m, cur - m, prev - m, Vec3(0, -1, 0));
        writer.pushTriangle(prev + m, cur + m, next + m, Vec3(0, 1, 0));

        // Unlink node
        delete currentNode;

        prevNode->next = nextNode;
        nextNode->prev = prevNode;
        currentNode = prevNode;
    }

    std::cout << "Total triangles: " << writer.getTriangleCount() << std::endl;
    std::cout << "Done, freeing object pool" << std::endl;

    pool.front = currentNode;
    pool.revertCircular();
    writer.write(string(argv[1]) + ".stl", true);

    return 0;
}
