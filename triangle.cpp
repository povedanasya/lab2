#include "triangle.h"
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

double sideLengths(const Point& p1, const Point& p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

double Triangle::gaussArea() const {
    return fabs(
        (A.x * (B.y - C.y) +
         B.x * (C.y - A.y) +
         C.x * (A.y - B.y)) / 2.0
    );
}

double Triangle::crossProductArea() const {
    double abx = B.x - A.x, aby = B.y - A.y;
    double acx = C.x - A.x, acy = C.y - A.y;
    return fabs(abx * acy - aby * acx) / 2.0;
}

double Triangle::heronArea() const {
    double a = sideLengths(A, B);
    double b = sideLengths(B, C);
    double c = sideLengths(C, A);

    double p = (a + b + c) / 2.0;

    return sqrt(p * (p - a) * (p - b) * (p - c));
}

bool Triangle::isDegenerate() const {
    double cross = (B.x - A.x) * (C.y - A.y)
                 - (B.y - A.y) * (C.x - A.x);

    double lenAB = sideLengths(A, B);
    double lenAC = sideLengths(A, C);

    return fabs(cross) < 1e-9 * lenAB * lenAC;
}

static bool pointOnSegment(const Point& V1, const Point& V2, const Point& P) {
    double cross = (V2.x - V1.x) * (P.y - V1.y)
                 - (V2.y - V1.y) * (P.x - V1.x);

    double segLen = sideLengths(V1, V2);

    if (fabs(cross) > 1e-6 * segLen) return false;

    if (P.x < fmin(V1.x, V2.x) - 1e-9 || P.x > fmax(V1.x, V2.x) + 1e-9) return false;
    if (P.y < fmin(V1.y, V2.y) - 1e-9 || P.y > fmax(V1.y, V2.y) + 1e-9) return false;

    return true;
}

bool Triangle::onEdge(const Point& P) const {
    return pointOnSegment(A, B, P)
        || pointOnSegment(B, C, P)
        || pointOnSegment(C, A, P);
}

bool Triangle::containsCross(const Point& P) const {
    if (isDegenerate()) return false;

    double s1 = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
    double s2 = (C.x - B.x) * (P.y - B.y) - (C.y - B.y) * (P.x - B.x);
    double s3 = (A.x - C.x) * (P.y - C.y) - (A.y - C.y) * (P.x - C.x);

    bool allPos = (s1 >= -1e-9) && (s2 >= -1e-9) && (s3 >= -1e-9);
    bool allNeg = (s1 <=  1e-9) && (s2 <=  1e-9) && (s3 <=  1e-9);

    return allPos || allNeg;
}

bool Triangle::containsAreaSum(const Point& P) const {
    if (isDegenerate()) return false;

    Triangle t1 = {A, B, P};
    Triangle t2 = {B, C, P};
    Triangle t3 = {C, A, P};

    double sumS = t1.gaussArea() + t2.gaussArea() + t3.gaussArea();
    double totalS = gaussArea();

    double eps = 1e-9 * totalS + 1e-15;

    return fabs(sumS - totalS) < eps;
}

static void printResult(const string& method, bool edge, bool inside) {
    cout << "  [" << method << "] ";

    if (edge)
        cout << "Point lies on the edge of the triangle" << endl;
    else if (inside)
        cout << "Point is inside the triangle" << endl;
    else
        cout << "Point is outside the triangle" << endl;
}

void runTriangle() {
    Triangle tri;

    cout << "=== Triangle Point Membership Check ===" << endl;

    cout << "Enter triangle vertex coordinates:" << endl;

    cout << "  Vertex A (x y): ";
    cin >> tri.A.x >> tri.A.y;

    cout << "  Vertex B (x y): ";
    cin >> tri.B.x >> tri.B.y;

    cout << "  Vertex C (x y): ";
    cin >> tri.C.x >> tri.C.y;

    cout << endl;

    if (tri.isDegenerate()) {
        cout << "ERROR: The triangle is degenerate!" << endl;
        return;
    }

    double ab = sideLengths(tri.A, tri.B);
    double bc = sideLengths(tri.B, tri.C);
    double ca = sideLengths(tri.C, tri.A);

    cout << "Area (Gauss):         " << tri.gaussArea() << endl;
    cout << "Area (cross product): " << tri.crossProductArea() << endl;
    cout << "Area (Heron):         " << tri.heronArea() << endl;

    cout << "Sides: AB=" << ab << "  BC=" << bc << "  CA=" << ca << endl;
    cout << "Perimeter: " << ab + bc + ca << endl;

    int n;
    cout << "\nNumber of points to check: ";
    cin >> n;

    for (int i = 1; i <= n; i++) {
        Point p;
        cout << "\nPoint " << i << " (x y): ";
        cin >> p.x >> p.y;

        bool edge = tri.onEdge(p);

        printResult("Cross product signs  ", edge, tri.containsCross(p));
        printResult("Sub-triangle area sum", edge, tri.containsAreaSum(p));
    }
}