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
    double crossProductArea() const;
    double heronArea() const;

    bool isDegenerate() const;
    bool containsCross(const Point& P) const;
    bool containsAreaSum(const Point& P) const;
    bool onEdge(const Point& P) const;
};

double sideLengths(const Point& p1, const Point& p2);

void runTriangle();

#endif