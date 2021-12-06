#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

struct Submarine
{
    int position = 0;
    int depth = 0;
    int aim = 0;

    void down(int units)
    {
        aim += units;
    }

    void up(int units)
    {
        aim -= units;
    }

    void forward(int units)
    {
        position += units;
        depth += (aim * units);
    }
};

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;
    std::stringstream line_stream;

    Submarine sub;

    std::string command;
    std::string sunit;
    int unit = 0;

    while (std::getline(fs, line))
    {
        // parse command
        line_stream = std::stringstream(line);
        std::getline(line_stream, command, ' ');
        std::getline(line_stream, sunit);
        unit = std::stoi(sunit);

        // apply it
        if (command == "up")
            sub.up(unit);
        if (command == "down")
            sub.down(unit);
        if (command == "forward")
            sub.forward(unit);
    };

    std::cout << sub.position * sub.depth << std::endl;
    return 0;
}