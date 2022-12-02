#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <assert.h>
#include <algorithm>

struct HeightMap;

struct Bassin
{
    int size = 0;
    bool operator<(Bassin &other)
    {
        return size < other.size;
    }
};
struct HeightPoint
{
    HeightMap *pMap;
    unsigned int height;
    int x = -1, y = -1;
    Bassin *pBassin = nullptr;

    bool operator<(HeightPoint &other) { return height < other.height; }
    bool is_border() { return height == 10; }
    bool mark_bassin(Bassin *pBassin)
    {
        if (is_border() || height == 9 || this->pBassin != nullptr)
            return false;

        this->pBassin = pBassin;
        pBassin->size++;
        return true;
    }
};

struct HeightMap
{
    std::vector<std::vector<HeightPoint>> points;

    HeightMap(std::fstream &stream)
    {
        std::string line_string;

        std::getline(stream, line_string);
        size_t line_length = line_string.size() + 2;

        points.push_back(std::vector<HeightPoint>(line_length, {this, 10}));
        int x = 1;
        do
        {
            std::vector<HeightPoint> line;
            line.push_back({this, 10});
            int y = 1;
            for (auto &c : line_string)
            {
                line.push_back({this, (unsigned int)(c - '0'), x, y});
                y++;
            }
            line.push_back({this, 10});
            points.push_back(line);
            x++;
        } while (std::getline(stream, line_string));

        points.push_back(std::vector<HeightPoint>(line_length, {this, 10}));
    }

    HeightPoint *at(size_t x, size_t y)
    {
        return &points[x][y];
    }

    std::vector<HeightPoint *> detect_low_points()
    {
        int risk = 0;
        std::vector<HeightPoint *> low_points;
        for (int x = 1; x < points.size() - 1; x++)
        {
            for (int y = 1; y < points[x].size() - 1; y++)
            {
                auto &point = points[x][y];
                if (point < points[x - 1][y] && point < points[x + 1][y] && point < points[x][y - 1] && point < points[x][y + 1])
                {
                    low_points.push_back(&point);
                    risk += (point.height + 1);
                }
            }
        }
        std::cout << "Total risk: " << risk << std::endl;
        return low_points;
    }

    void mark_bassin(Bassin *bassin, HeightPoint *low_point)
    {
        if (low_point->mark_bassin(bassin))
        {
            auto &x = low_point->x;
            auto &y = low_point->y;

            mark_bassin(bassin, &points[x - 1][y]);
            mark_bassin(bassin, &points[x + 1][y]);
            mark_bassin(bassin, &points[x][y - 1]);
            mark_bassin(bassin, &points[x][y + 1]);
        }
    }
};

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line_string;

    HeightMap map(fs);
    auto low_points = map.detect_low_points();

    std::vector<Bassin> bassins;
    for (auto &point : low_points)
    {
        bassins.push_back({});
        map.mark_bassin(&bassins.back(), point);
    }

    std::sort(bassins.begin(), bassins.end());
    auto it = bassins.rbegin();
    int prod = 1;
    for (int i = 0; i < 3; i++)
    {
        prod *= it->size;
        it++;
    }
    std::cout << "prod. size of 3 largest bassins: " << prod << std::endl;
}
