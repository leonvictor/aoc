#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <map>

using sign_t = uint8_t;
using outcome_t = uint8_t;
using round_t = std::pair<sign_t, sign_t>;
using tricked_round_t = std::pair<sign_t, outcome_t>;

namespace sign
{
    static sign_t Rock = 1;
    static sign_t Paper = 2;
    static sign_t Scissors = 3;
};

namespace outcome
{
    static outcome_t Win = 6;
    static outcome_t Lose = 0;
    static outcome_t Draw = 3;
};

std::map<tricked_round_t, sign_t> trick_rounds = {
    {{sign::Rock, outcome::Draw}, sign::Rock},
    {{sign::Rock, outcome::Win}, sign::Paper},
    {{sign::Rock, outcome::Lose}, sign::Scissors},

    {{sign::Paper, outcome::Draw}, sign::Paper},
    {{sign::Paper, outcome::Win}, sign::Scissors},
    {{sign::Paper, outcome::Lose}, sign::Rock},

    {{sign::Scissors, outcome::Draw}, sign::Scissors},
    {{sign::Scissors, outcome::Win}, sign::Rock},
    {{sign::Scissors, outcome::Lose}, sign::Paper},
};

std::map<char, sign_t> sign_map = {
    {'A', sign::Rock},
    {'X', sign::Rock},
    {'B', sign::Paper},
    {'Y', sign::Paper},
    {'C', sign::Scissors},
    {'Z', sign::Scissors},
};

std::map<char, outcome_t> outcome_map = {
    {'X', outcome::Lose},
    {'Y', outcome::Draw},
    {'Z', outcome::Win},
};

uint8_t winner(uint8_t opponent, uint8_t mine)
{
    if (opponent == mine)
    {
        return 3;
    }

    if (opponent == sign::Rock)
    {
        if (mine == sign::Paper)
        {
            return 6;
        }
        return 0;
    }
    if (opponent == sign::Paper)
    {
        if (mine == sign::Scissors)
        {
            return 6;
        }
        return 0;
    }
    if (mine == sign::Rock)
    {
        return 6;
    }

    return 0;
};

uint8_t scoring(uint8_t opponent, uint8_t mine)
{
    auto score = winner(opponent, mine);
    score += mine;
    return score;
};

int main()
{

    std::fstream fs("input", std::fstream::in);

    std::string line;
    uint32_t score = 0;
    uint32_t trick_score = 0;
    while (std::getline(fs, line))
    {
        // pt. 1
        score += scoring(sign_map[line[0]], sign_map[line[2]]);

        // pt. 2
        auto opponent = sign_map[line[0]];
        auto outcome = outcome_map[line[2]];
        auto mine = trick_rounds[tricked_round_t(opponent, outcome)];
        trick_score += (outcome + mine);
    }

    std::cout << "score: " << score << std::endl;
    std::cout << "tricked score: " << trick_score << std::endl;
    return 0;
};
