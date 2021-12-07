#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 10;

struct Pos
{
    int x;
    int y;

    void operator+=(Pos &other)
    {
        x += other.x;
        y += other.y;
    }

    void operator-=(Pos &other)
    {
        x -= other.x;
        y -= other.y;
    }
};

std::ostream &operator<<(std::ostream &os, Pos const &pos)
{
    os << pos.x << "," << pos.y;
    return os;
}

Pos operator+(Pos &rh, Pos &lh)
{
    return {rh.x + lh.x, rh.y + lh.y};
}

Pos operator-(Pos &rh, Pos &lh)
{
    return {rh.x - lh.x, rh.y - lh.y};
}

struct VentLine
{
    Pos start;
    Pos end;

    std::vector<Pos> get_atomic_displacements()
    {
        std::vector<Pos> atomics;

        auto disp = end - start;
        if (disp.y == 0)
        {
            int sign = disp.x / abs(disp.x);
            for (int i = 0; i < abs(disp.x); i++)
            {
                atomics.push_back({sign, 0});
            }
        }
        if (disp.x == 0)
        {
            int sign = disp.y / abs(disp.y);
            for (int i = 0; i < abs(disp.y); i++)
            {
                atomics.push_back({0, sign});
            }
        }

        return atomics;
    }
};

std::ostream &operator<<(std::ostream &os, VentLine const &vent_line)
{
    os << vent_line.start << " -> " << vent_line.end;
    return os;
}

struct Map
{
    int flags[MAP_WIDTH][MAP_HEIGHT];

    Map()
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            for (int y = 0; y < MAP_HEIGHT; y++)
            {
                flags[x][y] = 0;
            }
        }
    }

    void flag_line(VentLine &vent_line)
    {
        auto displacements = vent_line.get_atomic_displacements();
        if (displacements.size() == 0)
        {
            std::cout << "ignored (no displacement)" << std::endl;
            return;
        }

        Pos current_pos = vent_line.start;
        flag(current_pos);

        for (auto &disp : displacements)
        {
            current_pos += disp;
            flag(current_pos);
        }
    }

    void flag(int x, int y) { flags[x][y] += 1; }
    void flag(Pos pos) { flag(pos.x, pos.y); }

    int count_overlaps()
    {
        int count = 0;
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            for (int y = 0; y < MAP_HEIGHT; y++)
            {
                if (flags[x][y] > 1)
                {
                    count++;
                }
            }
        }
        return count;
    }

    void debug_draw()
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            for (int y = 0; y < MAP_HEIGHT; y++)
            {
                if (flags[x][y] > 0)
                {
                    std::cout << flags[x][y] << " ";
                }
                else
                {
                    std::cout << ". ";
                }
            }
            std::cout << std::endl;
        }
    }
};

Pos read_pos(std::stringstream &strm)
{

    std::string string_x, string_y;
    std::getline(strm, string_x, ',');
    std::getline(strm, string_y, ' ');

    Pos pos = {std::stoi(string_x), std::stoi(string_y)};
    return pos;
}

int main()
{
    std::vector<VentLine> vent_lines;

    std::fstream fs("input_example", std::fstream::in);
    std::string line;

    auto line_stream = std::stringstream(line);

    int max_x = 0, max_y = 0;
    while (std::getline(fs, line))
    {
        line_stream = std::stringstream(line);
        Pos start_pos = read_pos(line_stream);
        max_x = std::max(max_x, start_pos.x);
        max_y = std::max(max_y, start_pos.y);

        std::getline(line_stream, line, ' ');

        Pos end_pos = read_pos(line_stream);
        max_x = std::max(max_x, end_pos.x);
        max_y = std::max(max_y, end_pos.y);

        vent_lines.push_back({start_pos, end_pos});
    }

    Map map;
    map.debug_draw();
    for (auto &vent_line : vent_lines)
    {
        std::cout << std::endl
                  << vent_line
                  << std::endl;

        map.flag_line(vent_line);
        // map.debug_draw();
    }

    map.debug_draw();
    std::cout << map.count_overlaps() << " overlaps.";

    return 0;
}