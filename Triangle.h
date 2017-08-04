#ifndef Triangle_H
#define Traingle_H

#include <vector>
#include "SQMatrix.h"

struct Point
{
    float x, y, z;
};

class Triangle
{
public:
    Triangle();

    void SetCornerPoints(int, int, int);
    void GetCornerPoints(int&, int&, int&);
    void SetEdgePoints(int, int, int);
    void GetEdgePoints(int&, int&, int&);

    void ComputeCoeffs(std::vector<Point>&);

private:
    // indices of points
    int m_vx[6];

    // coefficients for the quadratic surface
    float m_A, m_B, m_C, m_D, m_E, m_F;

    // matrices
    SQMatrix m_L, m_M;
};

#endif // Triangle_H
