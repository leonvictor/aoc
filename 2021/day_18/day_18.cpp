#include <string>
#include <iostream>
#include <fstream>
#include <variant>
#include <memory>
#include <vector>

#include "snailfish_number.cpp"

snailfish_number::atomic_t parse_number(std::string::iterator &it)
{
    if (*it >= '0' && *it <= '9')
    {
        snailfish_number::regular_t value = *it - '0';
        ++it;
        return value;
    }
    else
    {
        auto first = parse_number(++it);
        auto second = parse_number(++it);

        auto number = std::make_shared<snailfish_number>();
        number->first = std::move(first);
        number->second = std::move(second);

        ++it;

        return number;
    }
}

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    std::vector<std::shared_ptr<snailfish_number>> numbers;

    std::getline(fs, line);
    auto it = line.begin();
    numbers.push_back(std::get<std::shared_ptr<snailfish_number>>(parse_number(it)));
    auto sum = numbers.back();

    while (std::getline(fs, line))
    {
        auto it = line.begin();
        numbers.push_back(std::get<std::shared_ptr<snailfish_number>>(parse_number(it)));
        sum = sum + numbers.back();
    }

    // p1
    std::cout << "magnitude: " << get_magnitude(sum) << std::endl;

    // p2
    snailfish_number::regular_t max_magnitude = 0ull;
    for (size_t i = 0; i < numbers.size() - 1; ++i)
    {
        for (size_t j = i + 1; j < numbers.size(); ++j)
        {
            auto a = numbers[i] + numbers[j];
            auto b = numbers[j] + numbers[i];
            max_magnitude = std::max(max_magnitude, std::max(get_magnitude(a), get_magnitude(b)));
        }
    }

    std::cout << "max magnitude: " << max_magnitude << std::endl;
    return 0;
};
