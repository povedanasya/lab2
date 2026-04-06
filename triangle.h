#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cmath>
#include <iostream>

struct Point {
    double x, y;
};

struct Triangle {
    Point A, B, C;

    double gaussArea() const;
    double heronArea() const;
    bool onEdge(const Point& P) const;
    bool containsCross(const Point& P) const;
    bool containsByArea(const Point& p) const;
};

double sideLengths(const Point& p1, const Point& p2);

void runTriangle();

#endif