#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <set>

struct cube
{
    std::pair<uint64_t, uint64_t> x;
    std::pair<uint64_t, uint64_t> y;
    std::pair<uint64_t, uint64_t> z;

    /// @brief returns a list of cubes that compose the parts of this cube that *do not* intersect with cuboid.
    /// if no intersection, returns a vector containing only this.
    std::vector<cube> difference(cube &cuboid)
    {
        struct cube_coord
        {
            uint64_t value;
            std::string id;
        };

        auto comp = [](const cube_coord &a, const cube_coord &b)
        {
            return a.value < b.value;
        };
        std::set<cube_coord, decltype(comp)> xs(comp);
        xs.insert({x.first, "1"});
        xs.insert({x.second, "1"});
        xs.insert({cuboid.x.first, "2"});
        xs.insert({cuboid.x.first, "2"});
        std::set<cube_coord, decltype(comp)> ys(comp);
        ys.insert({y.first, "1"});
        ys.insert({y.second, "1"});
        ys.insert({cuboid.y.first, "2"});
        ys.insert({cuboid.y.first, "2"});
        std::set<cube_coord, decltype(comp)> zs(comp);
        zs.insert({z.first, "1"});
        zs.insert({z.second, "1"});
        zs.insert({cuboid.z.first, "2"});
        zs.insert({cuboid.z.first, "2"});

        // xs = {}
        // detect the case where cuboid is wholly contained
        if (x.first < cuboid.x.first && x.second > cuboid.x.second ||
            y.first < cuboid.y.first && y.second > cuboid.y.second ||
            z.first < cuboid.z.first && z.second > cuboid.z.second)
        {
            std::cout << "I hoped that this case wouldn't happen" << std::endl;
        }
        else if (x.first > cuboid.x.first &&)

            // generate sub-cubes

            // if no intersection...
            return {*this};
    }

    uint64_t get_volume()
    {
        return (x.second - x.first) * (y.second - y.first) * (z.second - z.first);
    }
};

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    std::list<cube> on_cubes;

    while (std::getline(fs, line))
    {
        // todo: parse
        cube step;
        bool on;
        if (on)
        {
            std::list<cube> new_cubes = {step};
            for (auto it = on_cubes.begin(); it != on_cubes.end(); ++it)
            {
                for (auto it2 = new_cubes.begin(); it2 != new_cubes.end(); ++it2)
                {
                }
                auto diff = step.difference(*it);
            }
            on_cubes.insert(on_cubes.end(), new_cubes.begin(), new_cubes.end());
        }
        else
        {
            std::list<cube> new_cubes;
            for (auto it = on_cubes.begin(); it != on_cubes.end(); ++it)
            {
                auto diff = it->difference(step);
                if (diff.size() > 0)
                {
                    new_cubes.insert(new_cubes.end(), diff.begin(), diff.end());
                    on_cubes.erase(it);
                }
            }
            on_cubes.insert(on_cubes.end(), new_cubes.begin(), new_cubes.end());
        }
    }

    return 0;
};
