#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <sstream>
#include <vector>
#include <map>

using Polymere = std::list<char>;

struct Rule
{
    std::pair<char, char> pair;
    char insert;

    friend std::ostream &operator<<(std::ostream &, Rule &);
};

std::ostream &operator<<(std::ostream &stream, Rule &rule)
{
    return stream << rule.pair.first << rule.pair.second << " -> " << rule.insert;
}

int main()
{
    std::fstream fs("example_input", std::fstream::in);
    std::string line;

    Polymere polymere;
    std::getline(fs, line);

    for (auto &e : line)
    {
        polymere.push_back(e);
    }

    std::getline(fs, line);

    std::vector<Rule> rules;
    std::stringstream line_stream;
    while (std::getline(fs, line))
    {
        line_stream = std::stringstream(line);
        std::getline(line_stream, line, ' ');
        std::pair<char, char> pair(line[0], line[1]);

        std::getline(line_stream, line, ' ');

        std::getline(line_stream, line, ' ');
        char insert = line[0];
        rules.push_back({pair, line[0]});
    }

    uint8_t n_steps = 10;
    for (int i = 0; i < n_steps; i++)
    {
        std::vector<std::pair<std::list<char>::iterator, char>> inserted_elements;

        for (auto &rule : rules)
        {
            auto it_prev = polymere.begin();
            for (auto it = std::next(polymere.begin()); it != polymere.end(); it++)
            {
                if (*it_prev == rule.pair.first && *it == rule.pair.second)
                {
                    inserted_elements.push_back({it, rule.insert});
                }
                it_prev++;
            }
        }

        for (auto &[it, e] : inserted_elements)
        {
            polymere.emplace(it, e);
        }
    }

    std::map<char, int> occurences;
    for (auto &e : polymere)
    {
        auto pair = occurences.try_emplace(e);
        pair.first->second++;
    }

    int min = INT_MAX;
    int max = 0;
    for (auto &[e, count] : occurences)
    {
        min = std::min(min, count);
        max = std::max(max, count);
    }

    std::cout << max - min << std::endl;
    return 0;
};
