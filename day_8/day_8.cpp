#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

struct Entry
{
    std::vector<std::string> signal_patterns;
    std::vector<std::string> digit_values;
};

int main()
{
    std::fstream fs("example_input", std::fstream::in);
    std::string line;

    std::vector<Entry> entries;
    while (std::getline(fs, line))
    {
        Entry entry;

        auto line_stream = std::stringstream(line);
        while (std::getline(line_stream, line, ' ') && line != "|")
        {
            entry.signal_patterns.push_back(line);
        }
        while (std::getline(line_stream, line, ' '))
        {
            entry.digit_values.push_back(line);
        }
        entries.push_back(entry);
    }

    // populate the dictionnary
    std::map<char, std::vector<char>> candidates;
    for (char symbol = 'a'; symbol != 'h'; symbol++)
    {
        std::vector<char> tmp = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
        candidates.emplace(symbol, tmp);
    }

    int count = 0;
    for (auto &entry : entries)
    {
        std::string result = "0000";
        int digit_counter = 0;

        // part 1: count unique instances
        for (auto &digit : entry.digit_values)
        {
            if (digit.size() == 2) // 1
            {
                count++;
            }
            if (digit.size() == 4) // 4
            {
                count++;
            }
            if (digit.size() == 7) // 8
            {
                count++;
            }
            if (digit.size() == 3) // 7
            {
                count++;
            }
            else
            {
            }
            digit_counter++;
        }
    }

    std::cout << count << " instances of digits that use a unique number of segments.";
    return 0;
}