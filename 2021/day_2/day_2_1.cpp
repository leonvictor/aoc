#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;
    std::stringstream line_stream;

    int position = 0;
    int depth = 0;

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
            depth -= unit;
        if (command == "down")
            depth += unit;
        if (command == "forward")
            position += unit;
    };

    std::cout << position * depth << std::endl;
    return 0;
}