
#include "Triangle.h"

Triangle::Triangle() : m_L(6), m_M(6)
{
}

void Triangle::GetCornerPoints(int& o_a, int& o_b, int& o_c)
{
    o_a = m_vx[0];
    o_b = m_vx[1];
    o_c = m_vx[2];
}

void Triangle::SetCornerPoints(int i_a, int i_b, int i_c)
{
    m_vx[0] = i_a;
    m_vx[1] = i_b;
    m_vx[2] = i_c;
}

void Triangle::GetEdgePoints(int& o_d, int& o_e, int& o_f)
{
    o_d = m_vx[3];
    o_e = m_vx[4];
    o_f = m_vx[5];
}

void Triangle::SetEdgePoints(int i_d, int i_e, int i_f)
{
    m_vx[3] = i_d;
    m_vx[4] = i_e;
    m_vx[5] = i_f;
}

void Triangle::ComputeCoeffs(std::vector<Point>& i_pts)
{
    for (int i = 0; i < 6; i++)
    {
        Point p = i_pts[m_vx[i]];
        m_L.SetElement(6, 1, 1.0);
        m_L.SetElement(6, 2, p.x);
        m_L.SetElement(6, 3, p.y);
        m_L.SetElement(6, 4, p.x * p.x);
        m_L.SetElement(6, 5, p.x * p.y);
        m_L.SetElement(6, 6, p.y * p.y);
    }
}
