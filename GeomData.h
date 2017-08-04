#ifndef GeomData_H
#define GeomData_H

#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>
#include "Triangle.h"

// this is for the unordered_map of point index pairs
struct EdgeHash
{
    std::size_t operator()(const std::pair<int, int>& p) const
    {
        long l1 = p.first;
        long l2 = p.second;
        long t = l2 * 1000000 + l1; // TODO better idea?
        return std::hash<long> {}(t);
    }
};

class GeomData
{
public:
    ~GeomData() = default;

    void LoadData(std::string);

    static GeomData& GetInstance();

    void Dump();

private:
    GeomData() = default;

    void AddEdgePointToList(std::unordered_map<std::pair<int,int>, Point, EdgeHash>&, int, int, Point);
    Point PointAverage(const Point&, const Point&) const;
    void EmplaceEdgePoints(std::unordered_map<std::pair<int,int>, Point, EdgeHash>&);

    static std::unique_ptr<GeomData> m_instance;
    static std::atomic_bool m_created;
    static std::mutex m_creationLock;

    std::vector<Point> m_points;
    std::vector<Triangle> m_triangles;
};

#endif // GeomData_H
