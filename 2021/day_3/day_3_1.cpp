#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <sstream>
#include <bitset>
#include <vector>

template <int N>
bool most_common_bit(const std::vector<std::bitset<N>> &report, const int index)
{
    int counts[2] = {};
    for (auto &diag : report)
    {
        counts[diag[index]]++;
    }
    return counts[0] > counts[1];
};

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
        diagnostic_report.push_back(std::bitset<12>(line));
    }

    std::bitset<num_bits> gamma_rate;
    for (int i = 0; i < num_bits; i++)
    {
        gamma_rate.set(i, most_common_bit<num_bits>(diagnostic_report, i));
    }

    auto epsilon_rate = gamma_rate;
    epsilon_rate.flip();

    std::cout << "power consumption: " << epsilon_rate.to_ulong() * gamma_rate.to_ulong() << std::endl;
    return 0;
};
