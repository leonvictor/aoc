#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <map>

class deterministic_dice
{
private:
    uint64_t m_value = 1;
    uint64_t m_roll_count = 0;

public:
    uint64_t roll()
    {
        ++m_roll_count;

        auto res = m_value;
        m_value = m_value == 100 ? 1 : m_value + 1;

        return res;
    }

    uint64_t get_roll_count() { return m_roll_count; }
};

struct player
{
    uint8_t position;
    uint64_t score = 0;

    void advance(uint64_t value)
    {
        position = ((value + position - 1) % 10) + 1;
        score += position;
    }

    bool play(deterministic_dice *p_dice)
    {
        uint64_t total_roll = 0;
        for (uint8_t i = 0; i < 3; i++)
        {
            auto t = p_dice->roll();
            total_roll += t;
        }

        advance(total_roll);
        return score >= 1000;
    }

    bool operator<(const player &other) const
    {
        return position < other.position && score < other.score;
    }
};

struct universe
{
    std::array<uint8_t, 10> roll_sources = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::array<uint64_t, 2> wins_counter = {0, 0};
    std::map<std::array<player, 2>, std::pair<uint64_t, uint64_t>> outcomes;

    void play(std::array<player, 2> &state)
    {
        for (auto i = 1; i <= 3; i++)
            for (auto j = 1; j <= 3; j++)
                for (auto k = 1; k <= 3; k++)
                    roll_sources[i + j + k]++;

        turn(state, 0, 1);
    }

    void turn(std::array<player, 2> state, uint8_t player_turn, uint64_t paths)
    {
        for (auto roll = 3; roll <= 9; roll++)
        {
            auto new_state = state;
            auto &player = new_state[player_turn];

            auto new_paths = paths * roll_sources[roll];

            player.advance(roll);

            if (player.score >= 21)
            {
                wins_counter[player_turn] += new_paths;
            }
            else
            {
                turn(new_state, (player_turn + 1) % 2, new_paths);
            }
        }
    }
};

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    std::array<player, 2> players;
    std::getline(fs, line);
    players[0] = {(uint8_t)(line.back() - '0')};
    std::getline(fs, line);
    players[1] = {(uint8_t)(line.back() - '0')};

    auto players_p2 = players;

    // p1
    deterministic_dice dice;
    bool done = false;
    while (!done)
    {
        for (int i = 0; i < 2; i++)
        {
            if (players[i].play(&dice))
            {
                done = true;
                std::cout << "losing player score: " << players[(i + 1) % 2].score << std::endl;
                std::cout << "rolls: " << dice.get_roll_count() << std::endl;
                std::cout << players[(i + 1) % 2].score * dice.get_roll_count() << std::endl;
                break;
            }
        }
    }

    // p2
    universe u;
    u.play(players_p2);
    std::cout << std::max(u.wins_counter[0], u.wins_counter[1]) << std::endl;
    return 0;
};
