#include <iostream>
using namespace std;

struct Point {
    double x, y;
};

double cross(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

//바리센트릭 좌표 판별
bool isInsideTriangle(Point A, Point B, Point C, Point P) {

    double areaABC = cross(A, B, C); //전체 면적

    if (areaABC == 0) 
        return false; //삼각형 아니면

    double wA = cross(P, B, C) / areaABC;
    double wB = cross(P, C, A) / areaABC;
    double wC = cross(P, A, B) / areaABC;

    return (wA >= 0 && wB >= 0 && wC >= 0);
}

int main() {

    Point A = { 0, 0 };
    Point B = { 5, 0 };
    Point C = { 2.5, 5 };

    Point P = { 2.5, 2 };

    if (isInsideTriangle(A, B, C, P))
        cout << "Inside\n";
    else
        cout << "Outside\n";

    return 0;
}