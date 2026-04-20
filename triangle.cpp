#include "triangle.h"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

double sideLengths(const Point& p1, const Point& p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

double Triangle::gaussArea() const {
    return fabs((A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y)) / 2.0);
}

double Triangle::crossProductArea() const {
    double abx = B.x - A.x;
    double aby = B.y - A.y;
    double acx = C.x - A.x;
    double acy = C.y - A.y;
    return fabs(abx * acy - aby * acx) / 2.0;
}

bool Triangle::isDegenerate() const {
    return gaussArea() < 1e-9;
}

static bool pointOnSegment(const Point& V1, const Point& V2, const Point& P) {
    double cross = (V2.x - V1.x) * (P.y - V1.y) - (V2.y - V1.y) * (P.x - V1.x);
    double segLen = sideLengths(V1, V2);
    if (fabs(cross) > 1e-7 * (segLen + 1.0)) return false; 
    
    if (P.x < min(V1.x, V2.x) - 1e-9 || P.x > max(V1.x, V2.x) + 1e-9) return false;
    if (P.y < min(V1.y, V2.y) - 1e-9 || P.y > max(V1.y, V2.y) + 1e-9) return false;

    return true;
}

bool Triangle::onEdge(const Point& P) const {
    return pointOnSegment(A, B, P) || pointOnSegment(B, C, P) || pointOnSegment(C, A, P);
}

bool Triangle::containsCross(const Point& P) const {
    double s1 = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
    double s2 = (C.x - B.x) * (P.y - B.y) - (C.y - B.y) * (P.x - B.x);
    double s3 = (A.x - C.x) * (P.y - C.y) - (A.y - C.y) * (P.x - C.x);

    bool allPos = (s1 >= -1e-9) && (s2 >= -1e-9) && (s3 >= -1e-9);
    bool allNeg = (s1 <=  1e-9) && (s2 <=  1e-9) && (s3 <=  1e-9);

    return allPos || allNeg;
}

bool Triangle::containsByArea(const Point& p) const {
    double areaPAB = Triangle{A, B, p}.gaussArea();
    double areaPBC = Triangle{B, C, p}.gaussArea();
    double areaPCA = Triangle{C, A, p}.gaussArea();
    return fabs(areaPAB + areaPBC + areaPCA - gaussArea()) < 1e-9;
}

void runTriangle() {
    char repeat;
    do {
        Triangle tri;
        cout << "\n==========================================" << endl;
        cout << "            TRIANGLE ANALYZER" << endl;
        cout << "==========================================" << endl;
        cout << "Enter vertex A (x y): "; cin >> tri.A.x >> tri.A.y;
        cout << "Enter vertex B (x y): "; cin >> tri.B.x >> tri.B.y;
        cout << "Enter vertex C (x y): "; cin >> tri.C.x >> tri.C.y;

        if (tri.isDegenerate()) {
            cout << "\n[!] STATUS: DEGENERATE (collinear)" << endl;
        } else {
            cout << "\n[OK] STATUS: VALID TRIANGLE" << endl;
        }

        int n;
        cout << "How many points to check: "; cin >> n;

        for (int i = 1; i <= n; i++) {
            Point p;
            cout << "\n  Point " << i << " (x y): "; cin >> p.x >> p.y;

            cout << "  Method 1 (Cross): ";
            if (tri.onEdge(p)) cout << "ON THE EDGE / LINE";
            else if (tri.containsCross(p)) cout << "INSIDE";
            else cout << "OUTSIDE";
            cout << endl;

            cout << "  Method 2 (Area):  ";
            if (tri.onEdge(p)) cout << "ON THE EDGE / LINE";
            else if (tri.containsByArea(p)) cout << "INSIDE";
            else cout << "OUTSIDE";
            cout << endl;
        }

        cout << "\n------------------------------------------" << endl;
        cout << "Analyze another triangle? (y/n): ";
        cin >> repeat;

    } while (repeat == 'y' || repeat == 'Y');

    cout << "Exiting... Bye!" << endl;
}