#include <string>
#include <iostream>
#include <fstream>
#include <stack>
#include <map>
#include <vector>
#include <algorithm>

struct delimiter
{
    char character;
    int score;
};

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    // this could be a bidirectionnal map
    std::map<char, delimiter> opening_delimiters;
    opening_delimiters.emplace(')', delimiter{'(', 3});
    opening_delimiters.emplace(']', delimiter{'[', 57});
    opening_delimiters.emplace('}', delimiter{'{', 1197});
    opening_delimiters.emplace('>', delimiter{'<', 25137});

    std::map<char, delimiter> closing_delimiters;
    closing_delimiters.emplace('(', delimiter{')', 1});
    closing_delimiters.emplace('[', delimiter{']', 2});
    closing_delimiters.emplace('{', delimiter{'}', 3});
    closing_delimiters.emplace('<', delimiter{'>', 4});

    unsigned int corrupt_score = 0;
    std::vector<uint64_t> completion_scores;
    while (std::getline(fs, line))
    {
        bool corrupted = false;
        std::stack<char> open_chunks = std::stack<char>();
        for (auto it = line.begin(); it != line.end() && !corrupted; it++)
        {
            auto character = *it;
            if (character == '(' || character == '[' || character == '{' || character == '<')
            {
                open_chunks.push(character);
            }
            else
            {
                auto &matching_del = opening_delimiters[character];
                auto &opening = open_chunks.top();
                if (opening != matching_del.character)
                {
                    corrupt_score += matching_del.score;
                    corrupted = true;
                }
                open_chunks.pop();
            }
        }

        if (!corrupted)
        {
            uint64_t score = 0;
            while (open_chunks.size() > 0)
            {
                score *= 5;
                auto &opening = open_chunks.top();
                score += closing_delimiters[opening].score;
                open_chunks.pop();
            }
            completion_scores.push_back(score);
        }
    }

    std::sort(completion_scores.begin(), completion_scores.end());
    std::cout << "total corruption score: " << corrupt_score << std::endl;
    std::cout << "total completion score: " << completion_scores[completion_scores.size() / 2] << std::endl;

    return 0;
};
