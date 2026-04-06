#include "triangle.h"
#include <cmath>
#include <iostream>

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

double Triangle::heronArea() const {
    double abx = B.x - A.x;
    double aby = B.y - A.y;
    double acx = C.x - A.x;
    double acy = C.y - A.y;
    return fabs(abx * acy - aby * acx) / 2.0;
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

bool Triangle::containsByArea(const Point& p) const{
    return abs(Triangle{A, B, p}.gaussArea() + 
        Triangle{B, C, p}.gaussArea() +
        Triangle{A, C, p}.gaussArea() - gaussArea()) < 10e-12;
}

bool Triangle::onEdge(const Point& P) const {
    return pointOnSegment(A, B, P)
        || pointOnSegment(B, C, P)
        || pointOnSegment(C, A, P);
}

bool Triangle::containsCross(const Point& P) const {
    double s1 = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
    double s2 = (C.x - B.x) * (P.y - B.y) - (C.y - B.y) * (P.x - B.x);
    double s3 = (A.x - C.x) * (P.y - C.y) - (A.y - C.y) * (P.x - C.x);

    bool allPos = (s1 >= -1e-9) && (s2 >= -1e-9) && (s3 >= -1e-9);
    bool allNeg = (s1 <=  1e-9) && (s2 <=  1e-9) && (s3 <=  1e-9);

    return allPos || allNeg;
}

void runTriangle() {
    Triangle tri;

    cout << "=== Triangle Area Calculator ===" << endl;
    cout << "Enter vertex coordinates:" << endl;

    cout << "  Vertex A (x y): ";
    cin >> tri.A.x >> tri.A.y;

    cout << "  Vertex B (x y): ";
    cin >> tri.B.x >> tri.B.y;

    cout << "  Vertex C (x y): ";
    cin >> tri.C.x >> tri.C.y;

    int n;
    cout << "\nHow many points to check: ";
    cin >> n;

    for (int i = 1; i <= n; i++) {
        Point p;
        cout << "Point " << i << " (x y): ";
        cin >> p.x >> p.y;

        if (tri.onEdge(p))
            cout << "  --> Point is ON THE EDGE of the triangle." << endl;
        else if (tri.containsCross(p))
            cout << "  --> Point is INSIDE the triangle." << endl;
        else
            cout << "  --> Point is OUTSIDE the triangle." << endl;

        if (tri.onEdge(p))
            cout << "  --> By area: Point is ON THE EDGE of the triangle." << endl;
        else if (tri.containsByArea(p))
            cout << "  --> By area: Point is INSIDE the triangle." << endl;
        else
            cout << "  --> By area: Point is OUTSIDE the triangle." << endl;
    }
}