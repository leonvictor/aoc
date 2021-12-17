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

using literal_t = uint64_t;
using bit_t = int;

literal_t bits2int(std::vector<bit_t>::iterator &it, uint64_t count)
{
    literal_t out = 0ll;
    for (uint64_t i = 0ll; i < count; ++i)
    {
        if (*it)
            out += (uint64_t)1 << (count - (uint64_t)1 - i);
        ++it;
    }
    return out;
}

literal_t bits2int(std::vector<bit_t> &bits)
{
    auto it = bits.begin();
    return bits2int(it, bits.size());
}

literal_t product_operator(std::vector<literal_t> &literals)
{
    literal_t result = 1;
    std::cout << "prod(";
    for (auto lit : literals)
    {
        if (result > UINT64_MAX / lit)
        {
            std::cout << "Aie" << std::endl;
        }
        result *= lit;
        std::cout << lit << ", ";
    }
    std::cout << ") = " << result << std::endl;
    return result;
}

literal_t sum_operator(std::vector<literal_t> &literals)
{
    std::cout << "sum(";
    literal_t result = 0ll;
    // return std::accumulate(literals.begin(), literals.end(), 0ll);
    for (auto lit : literals)
    {
        if (result > UINT64_MAX - lit)
        {
            std::cout << "Aie" << std::endl;
        }
        result += lit;
        std::cout << lit << ", ";
    }
    std::cout << ") = " << result << std::endl;
    return result;
}

literal_t min_operator(std::vector<literal_t> &literals)
{
    std::cout << "min(";
    literal_t result = UINT64_MAX;
    for (auto lit : literals)
    {
        result = result < lit ? result : lit;
        // result = std::min(result, lit);
        std::cout << lit << ", ";
    }
    std::cout << ") = " << result << std::endl;
    return result;
}

literal_t max_operator(std::vector<literal_t> &literals)
{
    literal_t result = 0;
    std::cout << "max(";
    for (auto lit : literals)
    {
        result = result > lit ? result : lit;
        std::cout << lit << ", ";
    }
    std::cout << ") = " << result << std::endl;
    return result;
}

literal_t greater_than_operator(std::vector<literal_t> &literals)
{
    assert(literals.size() == 2);
    std::cout << literals[0] << ">" << literals[1] << "=" << ((literal_t)literals[0] > literals[1]) << std::endl;
    return (literal_t)literals[0] > literals[1];
}

literal_t less_than_operator(std::vector<literal_t> &literals)
{
    assert(literals.size() == 2);
    std::cout << literals[0] << "<" << literals[1] << "=" << ((literal_t)literals[0] < literals[1]) << std::endl;
    return (literal_t)literals[0] < literals[1];
}

literal_t equals_operator(std::vector<literal_t> &literals)
{
    assert(literals.size() == 2);
    std::cout << literals[0] << "==" << literals[1] << " =" << ((literal_t)literals[0] == literals[1]) << std::endl;
    return (literal_t)literals[0] == literals[1];
}

class Packet
{
private:
    literal_t m_version;
    literal_t m_type_id;
    literal_t m_value;
    literal_t m_version_sum = 0ll;

    literal_t m_bit_size;

    std::vector<bit_t>::iterator m_it;

    using operator_func = std::function<literal_t(std::vector<literal_t> &)>;
    std::array<operator_func, 8> operators =
        {
            &sum_operator,
            &product_operator,
            &min_operator,
            &max_operator,
            nullptr, // should be nullptr
            &greater_than_operator,
            &less_than_operator,
            &equals_operator,
    };

public:
    Packet(std::vector<bit_t>::iterator start) : m_bit_size(0), m_it(start)
    {
        m_version = read_next_bits(3);
        m_version_sum += m_version;
        m_type_id = read_next_bits(3);

        if (m_type_id == 4)
        {
            m_value = read_literal_value();
        }
        else
        { // operators
            m_bit_size++;
            std::vector<literal_t> subpackets;
            if (*m_it)
            {
                ++m_it;
                auto subpacket_count = read_next_bits(11);
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
                auto read_end = m_bit_size + read_next_bits(15);
                while (m_bit_size < read_end)
                {
                    Packet packet(m_it);
                    m_bit_size += packet.get_size();
                    m_it = packet.end();
                    m_version_sum += packet.get_version_sum();
                    subpackets.push_back(packet.get_value());
                }
            }
            m_value = operators[m_type_id](subpackets);
        };
    }

    literal_t get_value() { return m_value; }
    literal_t get_size() { return m_bit_size; }
    literal_t get_version() { return m_version; }
    literal_t get_version_sum() { return m_version_sum; }
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

    literal_t read_next_bits(uint64_t count)
    {
        m_bit_size += count;
        return bits2int(m_it, count);
    }
};

class BITSReader
{
public:
    std::vector<bit_t> bits;
    std::vector<bit_t>::iterator m_reader;
    literal_t version_count = 0ll;

public:
    literal_t size = 0ll;
    BITSReader(std::string hex)
    {
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
    }

    literal_t read()
    {
        m_reader = bits.begin();
        auto outer_packet = Packet(m_reader);
        size = outer_packet.get_size();
        std::cout << "version sum: " << outer_packet.get_version_sum() << std::endl;
        return outer_packet.get_value();
    }
};

int main()
{
    std::string input_path = "input";

    std::fstream fs(input_path, std::fstream::in);
    std::string hex;

    std::getline(fs, hex);
    BITSReader reader(hex);
    auto value = reader.read();

    std::cout << "packet value: " << value << std::endl;
    std::cout << reader.size << " bits read over " << reader.bits.size() << std::endl;
    // not : 569548927
    // not : 569512283 (too low)
    // not : 739303887024 (too low)
    return 0;
};