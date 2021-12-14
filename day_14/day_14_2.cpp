#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <sstream>
#include <vector>
#include <map>

using ElementPair = std::pair<char, char>;

struct Rule
{
    ElementPair pair;
    char insert;

    friend std::ostream &operator<<(std::ostream &, Rule &);
};

std::ostream &operator<<(std::ostream &stream, Rule &rule)
{
    return stream << rule.pair.first << rule.pair.second << " -> " << rule.insert;
}

struct Polymere
{
    char first, last;
    std::map<ElementPair, uint64_t> pairs_count;

    void apply(std::vector<Rule> &rules)
    {
        std::map<ElementPair, uint64_t> modified_pairs;
        std::vector<ElementPair> reset_pairs;
        for (auto &rule : rules)
        {
            ElementPair first_pair = {rule.pair.first, rule.insert};
            ElementPair second_pair = {rule.insert, rule.pair.second};

            auto res = pairs_count.try_emplace(rule.pair, 0);
            if (res.first->second > 0)
            {
                auto first_res = modified_pairs.try_emplace(first_pair, 0);
                first_res.first->second += res.first->second;

                auto second_res = modified_pairs.try_emplace(second_pair, 0);
                second_res.first->second += res.first->second;

                reset_pairs.push_back(rule.pair);
            }
        }

        for (auto &pair : reset_pairs)
        {
            pairs_count[pair] = 0;
        }
        for (auto &[pair, count] : modified_pairs)
        {
            pairs_count[pair] += count;
        }
    }

    std::map<char, uint64_t> count_elements()
    {
        std::map<char, uint64_t> occurences;
        for (auto &[pair, count] : pairs_count)
        {
            auto first = occurences.try_emplace(pair.first, 0);
            first.first->second += count;
            auto second = occurences.try_emplace(pair.second, 0);
            second.first->second += count;
        }

        for (auto &[elem, count] : occurences)
        {
            occurences[elem] = count / 2;
        }
        occurences[first] += 1;
        occurences[last] += 1;

        return occurences;
    }
};

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    std::getline(fs, line);

    Polymere polymere;
    polymere.first = line.front();
    polymere.last = line.back();
    for (int i = 0; i < line.size() - 1; i++)
    {
        auto p = polymere.pairs_count.try_emplace({line[i], line[i + 1]}, 0);
        p.first->second++;
    }

    std::getline(fs, line);

    std::vector<Rule> rules;
    std::stringstream line_stream;
    while (std::getline(fs, line))
    {
        line_stream = std::stringstream(line);
        std::getline(line_stream, line, ' ');
        ElementPair pair(line[0], line[1]);

        std::getline(line_stream, line, ' ');

        std::getline(line_stream, line, ' ');
        char insert = line[0];
        rules.push_back({pair, line[0]});
    }

    uint8_t n_steps = 40;
    for (int i = 0; i < n_steps; i++)
    {
        polymere.apply(rules);
    }

    uint64_t min = UINT64_MAX;
    uint64_t max = 0;
    for (auto &[e, count] : polymere.count_elements())
    {
        min = std::min(min, count);
        max = std::max(max, count);
    }

    std::cout << max - min << std::endl;
    return 0;
};
