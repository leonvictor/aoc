#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

using priority_t = uint32_t;
using item_type_t = char;

priority_t to_priority(item_type_t type)
{
    if (type < 'a')
    {
        return (priority_t)(type - 'A' + 27);
    }
    return (priority_t)(type - 'a' + 1);
}

item_type_t to_type(priority_t priority)
{
    if (priority > 26)
    {
        return (item_type_t)priority + 'A' - 27;
    }
    return (item_type_t)priority - 1 + 'a';
}

item_type_t find_shared_type(std::vector<std::string> &groups)
{
    std::vector<std::array<int, 52>> maps;
    maps.resize(groups.size());
    for (size_t i = 0; i < groups.size(); ++i)
    {
        maps[i].fill(0);
        for (auto &c : groups[i])
        {
            maps[i][to_priority(c) - 1]++;
        }
    }

    for (int item_idx = 0; item_idx < 52; ++item_idx)
    {
        bool found_in_all = true;
        for (int map_idx = 0; found_in_all && map_idx < maps.size(); ++map_idx)
        {
            if (maps[map_idx][item_idx] == 0)
            {
                found_in_all = false;
            }
        }

        if (found_in_all)
        {
            return to_type(item_idx + 1);
        }
    }
    return -1;
};

int main()
{
    std::fstream fs("input", std::fstream::in);

    uint32_t bag_sum = 0;
    uint32_t group_sum = 0;
    std::vector<std::string> groups;
    std::vector<std::string> bag;

    std::string line;
    while (std::getline(fs, line))
    {
        // pt. 1
        bag.push_back(line.substr(0, line.size() / 2));
        bag.push_back(line.substr(line.size() / 2, line.size()));
        auto shared_type = find_shared_type(bag);
        auto priority = to_priority(shared_type);
        bag_sum += priority;

        bag.clear();

        // pt. 2
        groups.push_back(line);
        if (groups.size() == 3)
        {
            auto shared_type = find_shared_type(groups);
            auto priority = to_priority(shared_type);
            group_sum += priority;

            groups.clear();
        }
    }

    std::cout << "total bag priority: " << bag_sum << std::endl;
    std::cout << "total groups priority: " << group_sum << std::endl;

    return 0;
};
