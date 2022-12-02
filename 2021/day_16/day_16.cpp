#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <sstream>
#include <map>
#include <set>
#include <unordered_map>
#include <bitset>
#include <unordered_map>
#include <array>
#include <functional>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <limits>

using literal_t = uint64_t;
using bit_t = int;

literal_t bits2int(std::vector<bit_t>::iterator &it, uint64_t count)
{
    std::string str;
    for (uint64_t i = std::numeric_limits<literal_t>::min(); i < count; ++i)
    {
        str += *it ? '1' : '0';
        ++it;
    }
    return std::stoull(str, nullptr, 2);
}

literal_t bits2int(std::vector<bit_t> &bits)
{
    auto it = bits.begin();
    return bits2int(it, bits.size());
}

literal_t product_operator(std::vector<literal_t> &literals)
{
    return std::accumulate(literals.begin(), literals.end(), 1ll, std::multiplies<>());
}

literal_t sum_operator(std::vector<literal_t> &literals)
{
    return std::accumulate(literals.begin(), literals.end(), std::numeric_limits<literal_t>::min(), std::plus<>());
}

literal_t min_operator(std::vector<literal_t> &literals)
{
    literal_t result = std::numeric_limits<literal_t>::max();
    for (auto lit : literals)
    {
        result = result < lit ? result : lit;
    }
    return result;
}

literal_t max_operator(std::vector<literal_t> &literals)
{
    literal_t result = std::numeric_limits<literal_t>::min();
    for (auto lit : literals)
    {
        result = result > lit ? result : lit;
    }
    return result;
}

literal_t greater_than_operator(std::vector<literal_t> &literals)
{
    return literals[0] > literals[1];
}

literal_t less_than_operator(std::vector<literal_t> &literals)
{
    return literals[0] < literals[1];
}

literal_t equals_operator(std::vector<literal_t> &literals)
{
    return literals[0] == literals[1];
}

class Packet
{
private:
    uint8_t m_version;
    uint8_t m_type_id;
    literal_t m_value;
    uint32_t m_version_sum = std::numeric_limits<literal_t>::min();

    literal_t m_bit_size;

    std::vector<bit_t>::iterator m_it;

    using operator_func = std::function<literal_t(std::vector<literal_t> &)>;
    std::unordered_map<uint8_t, operator_func> operators =
        {
            {0, &sum_operator},
            {1, &product_operator},
            {2, &min_operator},
            {3, &max_operator},
            {5, &greater_than_operator},
            {6, &less_than_operator},
            {7, &equals_operator},
    };

public:
    Packet(std::vector<bit_t>::iterator start) : m_bit_size(0), m_it(start)
    {
        m_version = (uint8_t)read_raw_value(3);
        m_version_sum += m_version;
        m_type_id = (uint8_t)read_raw_value(3);

        if (m_type_id == 4)
        {
            m_value = read_literal_value();
        }
        else
        {
            m_bit_size++;
            std::vector<literal_t> subpackets;
            if (*m_it)
            {
                ++m_it;
                auto subpacket_count = read_raw_value(11);
                for (uint64_t i = 0; i < subpacket_count; ++i)
                {
                    Packet packet(m_it);
                    m_bit_size += packet.get_size();
                    m_it = packet.end();
                    m_version_sum += packet.get_version_sum();
                    subpackets.push_back(packet.get_value());
                }
            }
            else
            {
                ++m_it;
                auto sub_length = read_raw_value(15);
                auto bit_count = std::numeric_limits<literal_t>::min();
                while (bit_count < sub_length)
                {
                    Packet packet(m_it);
                    m_bit_size += packet.get_size();
                    bit_count += packet.get_size();
                    m_it += packet.get_size();
                    m_version_sum += packet.get_version_sum();
                    subpackets.push_back(packet.get_value());
                }
            }

            m_value = operators[m_type_id](subpackets);
        };
    }

    literal_t get_value() { return m_value; }
    literal_t get_size() { return m_bit_size; }
    uint8_t get_version() { return m_version; }
    uint32_t get_version_sum() { return m_version_sum; }
    std::vector<bit_t>::iterator end() { return m_it; }

    literal_t read_literal_value()
    {
        std::vector<bit_t> literal;
        bool last_group = false;
        do
        {
            last_group = !(*m_it);
            ++m_it;
            for (uint8_t i = 0; i < 4; ++i)
            {
                literal.push_back(*m_it);
                ++m_it;
            }
            m_bit_size += 5;
        } while (!last_group);

        return bits2int(literal);
    }

    literal_t read_raw_value(literal_t count)
    {
        m_bit_size += count;
        return bits2int(m_it, count);
    }
};

int main()
{
    std::string input_path = "input";

    std::fstream fs(input_path, std::fstream::in);
    std::string hex;
    std::vector<bit_t> bits;
    std::getline(fs, hex);

    bits.reserve(hex.size() * 4);
    for (auto &character : hex)
    {
        for (uint8_t i = 4; i != 0; --i)
        {
            auto c = character >= 'A' ? character - 'A' + ':' : character;
            bit_t b = (c & (1 << (i - 1))) >> (i - 1);
            bits.push_back(b);
        }
    }
    auto outer_packet = Packet(bits.begin());

    std::cout << "packet value: " << outer_packet.get_value() << std::endl;
    std::cout << "version sum: " << outer_packet.get_version_sum() << std::endl;
    return 0;
};