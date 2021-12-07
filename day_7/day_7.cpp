#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

struct Crabs
{
    std::vector<int> crabs_per_position;

    void add_crab(int position)
    {
        while (crabs_per_position.size() <= position)
        {
            crabs_per_position.push_back(0);
        }
        crabs_per_position[position]++;
    }

    // part 1
    int simple_cost_to(int target_pos)
    {
        int cost = 0;
        for (int i = 0; i < crabs_per_position.size(); i++)
        {
            cost += abs(target_pos - i) * crabs_per_position[i];
        }
        return cost;
    }

    // part 2
    int accurate_cost_to(int target_pos)
    {
        int cost = 0;
        for (int i = 0; i < crabs_per_position.size(); i++)
        {
            auto distance = abs(target_pos - i);
            int unit_cost = 0;
            for (int j = distance; j >= 0; j--)
            {
                unit_cost += j;
            }
            cost += unit_cost * crabs_per_position[i];
        }
        return cost;
    }
};
int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    Crabs crabs;
    while (std::getline(fs, line, ','))
    {
        crabs.add_crab(std::stoi(line));
    }

    // this is a naïve search method
    std::vector<int> costs;
    for (int i = 0; i < crabs.crabs_per_position.size(); i++)
    {
        // int c = crabs.simple_cost_to(i);
        int c = crabs.accurate_cost_to(i);
        costs.push_back(c);
    }

    std::cout << "min cost: " << *(std::min_element(costs.begin(), costs.end())) << std::endl;

    return 0;
}