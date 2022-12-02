#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <cmath>
#include <set>
#include <algorithm>
#include <list>
#include <span>
#include <array>
#include <unordered_set>

#include "../vec3.hpp"

using coord_t = int64_t;
using beacon_t = vec3<coord_t>;

uint64_t cantor_pairing(uint64_t a, uint64_t b)
{
    return (a + b) * (a + b + 1) + b;
}

template <>
struct std::hash<beacon_t>
{
    std::size_t operator()(const beacon_t &beacon) const
    {
        return cantor_pairing(cantor_pairing(beacon.x, beacon.y), beacon.z);
    }
};

// Simplified axis angles representation
template <Arithmetic T>
using axis_angles = std::array<std::pair<size_t, T>, 3>;

template <Arithmetic T>
vec3<T> rotate(const vec3<T> &in, const axis_angles<T> &rotation)
{
    vec3<T> out;
    out.x = in[rotation[0].first] * rotation[0].second;
    out.y = in[rotation[1].first] * rotation[1].second;
    out.z = in[rotation[2].first] * rotation[2].second;
    return out;
}

template <Arithmetic T>
vec3<T> translate(const vec3<T> &in, const vec3<T> &translation)
{
    return in + translation;
}

size_t count_intersections(const std::set<uint32_t> &first, const std::set<uint32_t> &second)
{
    std::set<uint32_t> out;
    std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), std::inserter(out, out.begin()));
    return out.size();
}

struct Scanner
{
    uint8_t id;
    std::unordered_set<beacon_t> beacons;
    std::map<const beacon_t *, std::set<uint32_t>> distances;

    vec3<coord_t> position;
    axis_angles<coord_t> rotation;

    /// @brief Register this scanner's beacons to match another one's
    /// @note Simplified Kabsch algorithm (https://en.wikipedia.org/wiki/Kabsch_algorithm)
    void register_with(Scanner &scanner)
    {
        // Find mappings betwenn both point clouds
        std::vector<std::pair<const beacon_t *, const beacon_t *>> mapping;
        mapping.reserve(beacons.size());
        for (auto &[beacon_1, distances_1] : scanner.distances)
        {
            for (auto &[beacon_2, distances_2] : distances)
            {
                auto intersection_count = count_intersections(distances_1, distances_2);

                if (intersection_count > 10)
                {
                    mapping.push_back({beacon_1, beacon_2});
                }
            }
        }

        assert(mapping.size() > 0);

        // Find centroids
        vec3<float> centroid_1, centroid_2;
        for (auto &[beacon_1, beacon_2] : mapping)
        {
            centroid_1 += *beacon_1;
            centroid_2 += *beacon_2;
        }

        auto s = mapping.size();
        centroid_1 /= s;
        centroid_2 /= s;

        auto [p1, p2] = *mapping.begin();

        vec3<coord_t> mod_1 = *p1 - centroid_1;
        vec3<coord_t> mod_2 = *p2 - centroid_2;
        auto abs_mod_1 = mod_1.abs();
        auto abs_mod_2 = mod_2.abs();

        // Simplification: find the rotation for a single point
        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                if (abs_mod_1[i] == abs_mod_2[j])
                {
                    auto rot = mod_2[j] != 0 ? std::floor(mod_1[i] / mod_2[j]) : 0;
                    rotation[i] = {j, rot};
                    break;
                }
            }
        }

        auto rotated_p2 = rotate(*p2, rotation);
        position = rotated_p2 - *p1;
    }

    size_t count_common_beacons(const Scanner &other) const
    {
        size_t count = 0;
        for (auto it1 = distances.begin(); it1 != distances.end(); ++it1)
        {
            for (auto it2 = other.distances.begin(); it2 != other.distances.end(); ++it2)
            {
                count = std::max(count, count_intersections(it1->second, it2->second));
            }
        }
        return count;
    }

    void generate_distances()
    {
        distances.clear();
        for (auto it1 = beacons.begin(), end = beacons.end(); it1 != end; ++it1)
        {
            for (auto it2 = it1; it2 != end; ++it2)
            {
                if (*it1 != *it2)
                {
                    auto dist = euclidean_distance(*it1, *it2);

                    auto res = distances.try_emplace(&(*it1), std::set<uint32_t>{});
                    res.first->second.insert(dist);

                    res = distances.try_emplace(&(*it2), std::set<uint32_t>{});
                    res.first->second.insert(dist);
                }
            }
        }
    }
};

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;
    std::getline(fs, line);

    std::vector<Scanner> scanners;

    uint8_t id;
    Scanner scanner{id};

    while (std::getline(fs, line))
    {
        if (line == "")
        {
            scanners.push_back(scanner);
            scanners.back().generate_distances();
            std::getline(fs, line); // skip header

            id++;
            scanner = Scanner{id};
        }
        else
        {
            std::stringstream sstream(line);
            std::getline(sstream, line, ',');
            auto x = std::stoi(line);
            std::getline(sstream, line, ',');
            auto y = std::stoi(line);
            std::getline(sstream, line, ',');
            auto z = std::stoi(line);
            scanner.beacons.insert({x, y, z});
        }
    }
    scanners.push_back(scanner);
    scanners.back().generate_distances();

    std::list<Scanner *> remaining_scanners;
    for (auto &scan : scanners)
    {
        remaining_scanners.push_back(&scan);
    }
    remaining_scanners.pop_front();

    Scanner main_scanner = scanners[0];
    while (!remaining_scanners.empty())
    {
        // Elect the remaining scanner with the most common neighbors
        auto it_elected = remaining_scanners.begin();
        uint32_t max_neighbors = 0;

        // todo: an improvement would be to only consider pairs of scanners (1 registered, 1 not)
        for (auto it_candidate = remaining_scanners.begin(); it_candidate != remaining_scanners.end(); ++it_candidate)
        {
            auto count = main_scanner.count_common_beacons(**it_candidate);
            if (max_neighbors < count)
            {
                max_neighbors = count;
                it_elected = it_candidate;
            }
        }
        // Match the scanner's beacons to the main ones
        (*it_elected)->register_with(main_scanner);

        // Transform all points and add them to the main scanner
        for (auto beacon : (*it_elected)->beacons)
        {
            auto scan_rot = (*it_elected)->rotation;
            auto scan_pos = (*it_elected)->position;
            beacon = translate(rotate(beacon, scan_rot), -scan_pos);
            main_scanner.beacons.insert(beacon);
        }
        main_scanner.generate_distances();

        // Remove the scanner
        remaining_scanners.erase(it_elected);
    }

    std::cout << main_scanner.beacons.size() << " beacons detected" << std::endl;

    // p2
    coord_t max_dist = 0;
    for (auto &scan_1 : scanners)
    {
        for (auto &scan_2 : scanners)
        {
            max_dist = std::max(max_dist, manhattan_distance(scan_1.position, scan_2.position));
        }
    }

    std::cout << "max manhattan distance between two scanners: " << max_dist << std::endl;
    return 0;
};
