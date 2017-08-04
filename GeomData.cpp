#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "GeomData.h"

std::unique_ptr<GeomData> GeomData::m_instance;
std::atomic_bool GeomData::m_created(false);
std::mutex GeomData::m_creationLock;

GeomData& GeomData::GetInstance()
{
    std::lock_guard<std::mutex> lg(m_creationLock);
    if (!m_created)
    {
        m_created = true;
        m_instance = std::unique_ptr<GeomData> (new GeomData());
    }

    return *m_instance;
}

void GeomData::LoadData(std::string i_fileName)
{
    std::ifstream f(i_fileName);
    if (!f.is_open())
    {
        std::cerr << "Cannot open file: " << i_fileName << std::endl;
        throw;
    }

    int faceNo, vertexNo;

    std::string line;
    while (!f.eof())
    {
        getline(f, line);

        size_t pos = line.find("element face ");
        if (pos != std::string::npos)
        {
            std::string numstr = line.substr(13);
            faceNo = ::atoi(numstr.c_str());
        }

        pos = line.find("element vertex ");
        if (pos != std::string::npos)
        {
            std::string numstr = line.substr(15);
            vertexNo = ::atoi(numstr.c_str());
        }

        if (line == "end_header")
            break;
    }

    for (int i = 0; i < vertexNo; i++)
    {
        Point p;
        getline(f, line);
        size_t pos1 = line.find(" ");
        if (pos1 == std::string::npos)
        {
            std::cerr << "Cannot parse vertex line nr. " << i << std::endl;
            throw;
        }
        std::string num = line.substr(0, pos1);
        p.x = ::atof(num.c_str());

        size_t pos2 = line.find(" ", pos1 + 1);
        if (pos2 == std::string::npos)
        {
            std::cerr << "Cannot parse vertex line nr. " << i << std::endl;
            throw;
        }
        num = line.substr(pos1 + 1, pos2 - pos1 - 1);
        p.y = ::atof(num.c_str());

        num = line.substr(pos2 + 1, line.size() - pos2);
        p.z = ::atof(num.c_str());

        m_points.push_back(p);
    }

    std::unordered_map<std::pair<int, int>, Point, EdgeHash> ep;

    for (int i = 0; i < faceNo; i++)
    {
        Triangle t;
        getline(f, line);
        size_t pos1 = line.find(" ");
        if (pos1 == std::string::npos)
        {
            std::cerr << "Cannot parse vertex line nr. " << i << std::endl;
            throw;
        }
        size_t pos2 = line.find(" ", pos1 + 1);
        if (pos2 == std::string::npos)
        {
            std::cerr << "Cannot parse vertex line nr. " << i << std::endl;
            throw;
        }
        std::string num = line.substr(pos1 + 1, pos2 - pos1 - 1);
        int aIdx = ::atoi(num.c_str());

        size_t pos3 = line.find(" ", pos2 + 1);
        if (pos3 == std::string::npos)
        {
            std::cerr << "Cannot parse vertex line nr. " << i << std::endl;
            throw;
        }
        num = line.substr(pos2 + 1, pos3 - pos2 - 1);
        int bIdx = ::atoi(num.c_str());
        AddEdgePointToList(ep, aIdx, bIdx, PointAverage(m_points[aIdx], m_points[bIdx]));

        num = line.substr(pos3 + 1, line.size() - pos3);
        int cIdx = ::atoi(num.c_str());
        AddEdgePointToList(ep, aIdx, cIdx, PointAverage(m_points[aIdx], m_points[cIdx]));
        AddEdgePointToList(ep, bIdx, cIdx, PointAverage(m_points[bIdx], m_points[cIdx]));

        t.SetCornerPoints(aIdx, bIdx, cIdx);

        m_triangles.push_back(t);
    }

    f.close();

    EmplaceEdgePoints(ep);

    for (auto& tr : m_triangles)
        tr.ComputeCoeffs(m_points);
}

void GeomData::AddEdgePointToList(std::unordered_map<std::pair<int,int>, Point, EdgeHash>& ep, int a, int b, Point p)
{
    if (a > b)
        std::swap(a, b);

    std::pair<int, int> pr = std::make_pair(a, b);
    auto it = ep.find(pr);
    if (it == ep.end())
        ep.insert(std::make_pair(pr, p));
}

Point GeomData::PointAverage(const Point& i_p1, const Point& i_p2) const
{
    Point res;
    res.x = (i_p1.x + i_p2.x) / 2.0;
    res.y = (i_p1.y + i_p2.y) / 2.0;
    res.z = (i_p1.z + i_p2.z) / 2.0;

    return res;
}

void GeomData::EmplaceEdgePoints(std::unordered_map<std::pair<int,int>, Point, EdgeHash>& ep)
{
    // index of the new point in m_points
    size_t newIdx = m_points.size();

    for (auto& tr : m_triangles)
    {
        int a, b, c;
        tr.GetCornerPoints(a, b, c);

        if (a > b)
            std::swap(a, b);
        if (b > c)
            std::swap(b, c);

        std::pair<int, int> pr = std::make_pair(a, b);
        auto it = ep.find(pr);
        m_points.push_back(it->second);

        pr = std::make_pair(a, c);
        it = ep.find(pr);
        m_points.push_back(it->second);

        pr = std::make_pair(b, c);
        it = ep.find(pr);
        m_points.push_back(it->second);

        tr.SetEdgePoints(newIdx, newIdx + 1, newIdx + 2);

        newIdx += 3;
    }
}

void GeomData::Dump()
{
    int counter = 0;
    for (auto& tr : m_triangles)
    {
        std::cout << "Triangle nr " << counter << std::endl;

        int vx[6];
        tr.GetCornerPoints(vx[0], vx[1], vx[2]);
        tr.GetEdgePoints(vx[3], vx[4], vx[5]);

        for (int i = 0; i < 6; i++)
            std::cout << m_points[vx[i]].x << " " << m_points[vx[i]].y << " " << m_points[vx[i]].z << std::endl;

        std::cout << std::endl;

        counter++;
    }
}
