#include <bitset>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    int count = 0;
    int total_sum = 0;
    while (std::getline(fs, line))
    {
        std::vector<std::bitset<7>> patterns;  // received input
        std::array<std::bitset<7>, 10> digits; // actual digits
        std::map<unsigned long, uint8_t> mapping;

        std::string pattern_string;
        auto line_stream = std::stringstream(line);
        while (std::getline(line_stream, pattern_string, ' ') && pattern_string != "|")
        {
            std::bitset<7> bitset;
            for (auto segment : pattern_string)
            {
                bitset.set(segment - 'a', true);
            }
            patterns.push_back(bitset);
        }

        const auto set_pattern = [&](std::bitset<7> pattern, uint8_t digit)
        {
            digits[digit] = pattern;
            mapping.emplace(pattern.to_ulong(), digit);
        };

        // start by mapping the known digits
        for (auto pattern : patterns)
        {
            switch (pattern.count())
            {
            case 2:
                set_pattern(pattern, 1);
                break;
            case 3:
                set_pattern(pattern, 7);
                break;
            case 4:
                set_pattern(pattern, 4);
                break;
            case 7:
                set_pattern(pattern, 8);
                break;
            default:
                break;
            }
        }

        // then the more complex ones
        for (auto pattern : patterns)
        {
            switch (pattern.count())
            {
            case 5:
            {

                if ((pattern & digits[1]) == digits[1])
                {
                    set_pattern(pattern, 3);
                }
                else if ((pattern | digits[4]) == digits[8])
                {
                    set_pattern(pattern, 2);
                }
                else
                {
                    set_pattern(pattern, 5);
                }
                break;
            }
            case 6:
            {
                if ((pattern & digits[4]) == digits[4])
                {
                    set_pattern(pattern, 9);
                }
                else if ((pattern & digits[1]) == digits[1])
                {
                    set_pattern(pattern, 0);
                }
                else
                {
                    set_pattern(pattern, 6);
                }
                break;
            }
            default:
                break;
            }
        }

        std::string result = "";
        while (std::getline(line_stream, pattern_string, ' '))
        {
            std::bitset<7> pattern;
            for (auto segment : pattern_string)
            {
                pattern.set(segment - 'a', true);
            }

            auto digit = mapping[pattern.to_ulong()];
            result += std::to_string(digit);
        }

        std::cout << result << std::endl;
        total_sum += std::stoi(result);
    }

    std::cout << "total sum: " << total_sum << std::endl;
    return 0;
}