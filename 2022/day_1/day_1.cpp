#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <algorithm>

int main()
{
    std::fstream fs("input", std::fstream::in);

    uint32_t count = 0;
    uint32_t sum = 0;
    std::array<uint32_t, 3> top_three = {0, 0, 0};

    std::string line;
    while (std::getline(fs, line))
    {
        if (line == "")
        {
            if (sum > top_three[0])
            {
                top_three[0] = sum;
                std::sort(top_three.begin(), top_three.end());
            }
            sum = 0;
        }
        else
        {
            count = std::stoi(line);
            sum += count;
        }
    }

    uint32_t max = 0;
    uint32_t total = 0;
    for (uint32_t i = 0; i < 3; ++i)
    {
        max = std::max(top_three[i], max);
        total += top_three[i];
    }
    std::cout << "max: " << max << std::endl;
    std::cout << "total top-three: " << total << std::endl;
    return 0;
};
