#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cstring>
#include <limits>

const std::string digits = "123456789";
const std::array<std::string, 9> spelled_out_digits =
    {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine",
};

bool is_digit(const char *string, const std::string &digit)
{
    return std::strncmp(string, digit.c_str(), digit.size()) == 0;
}

char find_first_digit(const std::string &line)
{
    size_t first_spelled_out_digit_index = std::numeric_limits<size_t>::max();
    size_t first_numerical_digit_idx = line.find_first_of(digits);

    bool digit_found = false;
    char spelled_out_digit = '0';
    for (uint8_t char_idx = 0; char_idx < line.size() && !digit_found && char_idx < first_numerical_digit_idx; ++char_idx)
    {
        const char *candidate = &line[char_idx];
        for (uint8_t digit_idx = 0; digit_idx < spelled_out_digits.size() && !digit_found; ++digit_idx)
        {
            const auto &digit = spelled_out_digits[digit_idx];

            if (is_digit(candidate, digit))
            {
                digit_found = true;
                first_spelled_out_digit_index = char_idx;
                spelled_out_digit = std::to_string(digit_idx + 1)[0];
            }
        }
    }

    if (first_spelled_out_digit_index < first_numerical_digit_idx)
    {
        return spelled_out_digit;
    }
    else
    {
        return line[first_numerical_digit_idx];
    }
}

char find_last_digit(const std::string &line)
{
    size_t last_spelled_out_digit_idx = 0;
    size_t last_numerical_digit_idx = line.find_last_of(digits);

    char spelled_out_digit_found = '0';

    for (size_t spelled_out_digit_idx = 0; spelled_out_digit_idx < spelled_out_digits.size(); ++spelled_out_digit_idx)
    {
        const auto &spelled_out_digit = spelled_out_digits[spelled_out_digit_idx];
        auto idx = line.rfind(spelled_out_digit);
        if (idx != std::string::npos && idx > last_spelled_out_digit_idx)
        {
            last_spelled_out_digit_idx = idx;
            spelled_out_digit_found = std::to_string(spelled_out_digit_idx + 1)[0];
        }
    }

    if (last_numerical_digit_idx == std::string::npos || last_spelled_out_digit_idx > last_numerical_digit_idx)
    {
        return spelled_out_digit_found;
    }
    else
    {
        return line[last_numerical_digit_idx];
    }
}

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    std::string value_string = "00";

    uint32_t calibration_values_sum = 0;
    while (std::getline(fs, line))
    {
        value_string[0] = find_first_digit(line);
        value_string[1] = find_last_digit(line);

        uint8_t calibration_value = std::stoi(value_string);
        calibration_values_sum += calibration_value;
    }

    std::cout << "calibration values sum: " << calibration_values_sum << std::endl;
    return 0;
};
