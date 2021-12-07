#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

int sign_fn(int x)
{
    if (x > 0)
    {
        return 1;
    }
    else if (x < 0)
    {
        return -1;
    }
    return 0;
}
struct Crabs
{
    std::vector<int> crabs_per_position;
    float mean_position = -1.0f;
    int num_crabs = -1;

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

    // optimized part 2
    int cost_using_mean(int target_pos)
    {
        auto sum = 0;
        for (int i = 0; i < crabs_per_position.size(); i++)
        {
            sum += sign_fn(crabs_per_position[i] - target_pos);
        }
        auto cost = get_mean_pos() - (1 / 2) * (sum / get_num_crabs());
        return cost;
    }

    int get_num_crabs()
    {
        if (num_crabs > 0)
            return num_crabs;

        get_mean_pos();
        return num_crabs;
    }

    float get_mean_pos()
    {
        if (mean_position < -1.0f)
            return mean_position;

        num_crabs = 0;
        int sum = 0;
        for (int i = 0; i < crabs_per_position.size(); i++)
        {
            num_crabs += crabs_per_position[i];
            sum += (crabs_per_position[i] * i);
        }
        mean_position = sum / num_crabs;
        return mean_position;
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

    std::vector<int> costs;
    // mean method : https://old.reddit.com/r/adventofcode/comments/rawxad/2021_day_7_part_2_i_wrote_a_paper_on_todays/
    for (int i = (int)(crabs.get_mean_pos() - (1 / 2)); i <= (int)(crabs.get_mean_pos() + (1 / 2)); i++)
    {
        int c = crabs.accurate_cost_to(i);
        costs.push_back(c);
    }

    std::cout << "min cost: " << *(std::min_element(costs.begin(), costs.end())) << std::endl;

    return 0;
}