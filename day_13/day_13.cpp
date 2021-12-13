#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_set>
#include <set>
#include <sstream>
#include <algorithm>

struct Dot
{
    int x, y;
    bool operator<(const Dot &other) const
    {
        if (y == other.y)
        {
            return x < other.x;
        }
        return y < other.y;
    }

    bool operator==(const Dot &b) const { return x == b.x && y == b.y; }
};

using Paper = std::set<Dot>;

void display(Paper &paper)
{
    int max_x = 0, max_y = 0;
    int min_x = 0, min_y = 0;
    for (auto it = paper.begin(); it != paper.end(); it++)
    {
        max_x = std::max(max_x, it->x);
        min_x = std::min(min_x, it->x);
        max_y = std::max(max_y, it->y);
        min_y = std::min(min_y, it->y);
    }

    auto it = paper.begin();
    for (int y = min_y; y < max_y + 1; y++)
    {
        for (int x = min_x; x < max_x + 1; x++)
        {
            if (it != paper.end() && it->x == x && it->y == y)
            {
                std::cout << "#";
                it++;
            }
            else
            {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;
    std::stringstream line_stream;

    Paper paper;
    while (std::getline(fs, line) && line != "")
    {
        line_stream = std::stringstream(line);
        std::getline(line_stream, line, ',');
        auto x = std::stoi(line);
        std::getline(line_stream, line);
        auto y = std::stoi(line);
        paper.emplace(Dot{x, y});
    }

    std::string axis;
    int value;
    while (std::getline(fs, line))
    {
        line_stream = std::stringstream(line);
        std::getline(line_stream, line, ' ');
        std::getline(line_stream, line, ' ');
        std::getline(line_stream, axis, '=');
        std::getline(line_stream, line);
        value = std::stoi(line);

        Paper folded;
        if (axis == "x")
        {
            for (auto &dot : paper)
            {
                if (dot.x < value)
                {
                    folded.insert(dot);
                }
                else
                {
                    folded.insert({value - (dot.x - value), dot.y});
                }
            }
        }
        else if (axis == "y")
        {
            for (auto &dot : paper)
            {
                if (dot.y < value)
                {
                    folded.insert(dot);
                }
                else
                {
                    folded.insert({dot.x, value - (dot.y - value)});
                }
            }
        }
        paper = folded;
        // break; // Part 1
    }

    std::cout << paper.size() << " dots remaining" << std::endl;

    display(paper);
    return 0;
};
