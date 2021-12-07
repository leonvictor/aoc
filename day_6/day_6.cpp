#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>

const int sim_steps = 256;
using fishcount_t = uint64_t;

void count_fishes(std::vector<fishcount_t> &fishes)
{
    fishcount_t count = 0;
    for (auto &timer : fishes)
    {
        count += timer;
    }
    std::cout << count << " fishes!" << std::endl;
}

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    std::vector<fishcount_t> fishes = std::vector<fishcount_t>(9, (fishcount_t)0);

    while (std::getline(fs, line, ','))
    {
        fishes[std::stoi(line)]++;
    }

    for (int i = 0; i < sim_steps; i++)
    {
        std::vector<fishcount_t> next_step;
        // decrease all internal timers
        for (int i = 1; i < fishes.size(); i++)
        {
            next_step.push_back(fishes[i]);
        }

        next_step[6] += fishes[0];      // reset fishes that gave birth
        next_step.push_back(fishes[0]); // spawn new fishes

        fishes = std::vector<fishcount_t>(next_step);
    }
    count_fishes(fishes);
    return 0;
};
