#include <string>
#include <iostream>
#include <fstream>
#include <queue>

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    // populate a window queue with the first 3 values
    std::queue<int> window;
    for (int i = 0; i < 3; i++)
    {
        std::getline(fs, line);
        window.push(std::stoi(line));
    }

    int count = 0;

    while (std::getline(fs, line))
    {
        // each window share two values so we only need to compare the first with the last
        window.push(std::stoi(line));
        if (window.front() < window.back())
        {
            count++;
        }
        window.pop();
    }

    std::cout << "count: " << count << std::endl;
    return 0;
};
