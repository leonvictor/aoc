#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "../matrix.hpp"

struct infinite_image
{
    std::vector<std::string> m_data;
    char m_background = '0';

    void enhance(std::vector<bool> &algo)
    {
        // increase borders
        m_data.insert(m_data.begin(), std::string(m_data[0].size(), m_background));
        m_data.push_back(std::string(m_data[0].size(), m_background));
        for (auto &row : m_data)
        {
            row.insert(row.begin(), m_background);
            row.push_back(m_background);
        }

        std::vector<std::string> data;
        data.reserve(m_data.size() + 2);
        for (int row_idx = 0; row_idx < m_data.size(); ++row_idx)
        {
            auto &row = m_data[row_idx];
            std::string new_row;
            new_row.reserve(row.size() + 2);
            for (int col_idx = 0; col_idx < row.size(); ++col_idx)
            {
                std::string window;
                for (int i = row_idx - 1; i <= row_idx + 1; ++i)
                {
                    for (int j = col_idx - 1; j <= col_idx + 1; ++j)
                    {
                        if (i < 0 || j < 0 || i >= m_data.size() || j >= row.size())
                        {
                            window += m_background;
                        }
                        else
                        {
                            window += m_data[i][j];
                        }
                    }
                }
                new_row += algo[std::stoi(window, nullptr, 2)] ? '1' : '0';
            }
            data.push_back(new_row);
        }

        m_data = data;

        // switch background according to input
        if (m_background == '0' && algo.front())
        {
            m_background = '1';
        }
        else if (m_background == '1' && !algo.back())
        {
            m_background = '0';
        }
    }

    size_t lit_pixels()
    {
        size_t count = 0;
        for (auto &row : m_data)
        {
            for (auto &p : row)
            {
                if (p == '1')
                {
                    count++;
                }
            }
        }
        return count;
    }
};

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    std::getline(fs, line);
    std::vector<bool> algo;
    algo.reserve(512);
    for (auto &c : line)
    {
        algo.push_back(c == '#');
    }
    assert(algo.size() == 512);

    std::getline(fs, line); // skip empty

    infinite_image img;
    while (std::getline(fs, line))
    {
        std::string row;
        for (auto p : line)
        {
            row.push_back(p == '#' ? '1' : '0');
        }
        img.m_data.push_back(row);
    }

    // p1
    for (auto i = 0; i < 2; i++)
    {
        img.enhance(algo);
    }
    std::cout << img.lit_pixels() << " pixels lit after 2 steps" << std::endl;

    // p2
    for (auto i = 0; i < 48; i++)
    {
        img.enhance(algo);
    }
    std::cout << img.lit_pixels() << " pixels lit after 50 steps" << std::endl;
    return 0;
};
