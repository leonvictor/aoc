#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <cstring>

struct Cave
{
    std::string name;
    bool is_big;
    std::vector<std::shared_ptr<Cave>> connected_caves;

    Cave(std::string name) : name(name)
    {
        is_big = std::isupper(name[0]);
    }
};

struct Path : public std::vector<std::shared_ptr<Cave>>
{
    bool visited_small_cave_twice = false;
};

struct Probe
{
    std::vector<Path> eligible_paths;

    std::vector<Path> visit(std::shared_ptr<Cave> cave, Path &leading_path)
    {
        std::vector<Path> available_paths = std::vector<Path>();
        if (cave->name == "end")
        {
            leading_path.push_back(cave);
            available_paths.push_back(leading_path);
            return available_paths;
        }

        if (!cave->is_big && std::find(leading_path.begin(), leading_path.end(), cave) != leading_path.end())
        {
            if (leading_path.visited_small_cave_twice || cave->name == "start" || cave->name == "end")
            {
                return available_paths;
            }
            else
            {
                leading_path.visited_small_cave_twice = true;
            }
        }

        for (auto &p : cave->connected_caves)
        {
            Path new_path = leading_path;
            new_path.push_back(cave);
            auto paths = visit(p, new_path);
            available_paths.insert(available_paths.end(), paths.begin(), paths.end());
        }

        return available_paths;
    }
};

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;
    std::stringstream line_stream;

    std::map<std::string, std::shared_ptr<Cave>> caves;
    std::shared_ptr<Cave> start = nullptr;

    while (std::getline(fs, line))
    {
        line_stream = std::stringstream(line);
        std::getline(line_stream, line, '-');
        auto first = caves.try_emplace(line, std::make_shared<Cave>(line));

        // save starting cave
        if (start == nullptr && line == "start")
            start = first.first->second;

        std ::getline(line_stream, line);
        auto second = caves.try_emplace(line, std::make_shared<Cave>(line));

        // create paths
        first.first->second->connected_caves.push_back(second.first->second);
        second.first->second->connected_caves.push_back(first.first->second);
    }

    Probe probe;
    auto paths = probe.visit(start, Path());
    std::cout << paths.size() << " available paths" << std::endl;

    return 0;
};
