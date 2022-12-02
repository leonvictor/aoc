#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <sstream>
#include <bitset>
#include <vector>

template <int N>
void count_common_bits(const std::vector<std::bitset<N>> &report, const size_t index, int *out)
{
    for (auto &diag : report)
    {
        auto res = static_cast<int>(diag[index]);
        out[res]++;
    }
}

template <int N>
bool most_common_bit(const std::vector<std::bitset<N>> &report, const size_t index)
{
    int counts[2] = {0, 0};
    count_common_bits<N>(report, index, counts);
    return counts[0] <= counts[1];
};

template <int N>
bool least_common_bit(const std::vector<std::bitset<N>> &report, const size_t index)
{
    int counts[2] = {0, 0};
    count_common_bits<N>(report, index, counts);
    return counts[0] > counts[1];
};

template <int N>
std::bitset<N> reverse(std::bitset<N> &bitset)
{
    std::string bit_string = bitset.to_string();
    return std::bitset<N>(std::string(bit_string.rbegin(), bit_string.rend()));
};

template <int N>
std::vector<std::bitset<N>> get_matching_reports(const std::vector<std::bitset<N>> &reports, const size_t index, bool bit_criteria)
{
    std::vector<std::bitset<N>> matching_reports;
    for (auto &diag : reports)
    {
        if (diag[index] == bit_criteria)
        {
            matching_reports.push_back(diag);
        }
    }
    return matching_reports;
}

int main()
{
    const int num_bits = 12;
    const int num_diagnostics = 1000;

    std::fstream fs("input", std::fstream::in);
    std::string line;
    std::stringstream line_stream;

    std::vector<std::bitset<num_bits>> diagnostic_report;
    diagnostic_report.reserve(num_diagnostics);
    while (std::getline(fs, line))
    {
        diagnostic_report.push_back(std::bitset<num_bits>(std::string(line.rbegin(), line.rend())));
    }

    // oxygen
    bool found = false;
    auto matching_reports = std::vector<std::bitset<num_bits>>(diagnostic_report);
    for (size_t i = 0; !found && i < num_bits; i++)
    {
        bool bit_criteria = most_common_bit<num_bits>(matching_reports, i);
        matching_reports = get_matching_reports<num_bits>(matching_reports, i, bit_criteria);
        found = matching_reports.size() == 1;
    }

    std::bitset<num_bits> oxygen_generator_rating = reverse<num_bits>(matching_reports[0]);

    // co2
    found = false;
    matching_reports = std::vector<std::bitset<num_bits>>(diagnostic_report);
    for (size_t i = 0; !found && i < num_bits; i++)
    {
        bool bit_criteria = least_common_bit<num_bits>(matching_reports, i);
        matching_reports = get_matching_reports<num_bits>(matching_reports, i, bit_criteria);
        found = matching_reports.size() == 1;
    }
    std::bitset<num_bits> co2_scrubber_rating = reverse<num_bits>(matching_reports[0]);

    std::cout << "oxygen rating: " << oxygen_generator_rating << '\t' << oxygen_generator_rating.to_ulong() << std::endl;
    std::cout << "co2 rating: " << co2_scrubber_rating << '\t' << co2_scrubber_rating.to_ulong() << std::endl;
    std::cout << "life support rating: " << oxygen_generator_rating.to_ulong() * co2_scrubber_rating.to_ulong() << std::endl;
    return 0;
};
