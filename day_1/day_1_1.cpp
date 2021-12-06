#include <string>
#include <iostream>
#include <fstream>

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;
    std::getline(fs, line);

    int current = std::stoi(line);
    int previous;
    int count = 0;

    while (std::getline(fs, line))
    {
        previous = current;
        current = std::stoi(line);
        if (current > previous)
        {
            count++;
        }
    }

    std::cout << "count: " << count << std::endl;
    return 0;
};
