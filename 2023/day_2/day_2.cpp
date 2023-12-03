#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <sstream>

enum class cube_color : uint8_t
{
    red,
    green,
    blue,
};

std::pair<bool, uint64_t> process_game(const std::string &line, std::array<uint32_t, 3> &bag_contents)
{
    std::array<uint32_t, 3> min_required_cubes = {0, 0, 0};
    bool game_possible = true;

    std::stringstream line_stream = std::stringstream(line);

    // Skip header
    std::string header;
    std::getline(line_stream, header, ':');

    std::string cube_set;
    std::string draw;
    while (std::getline(line_stream, cube_set, ';'))
    {
        std::stringstream set_stream = std::stringstream(cube_set);
        while (std::getline(set_stream, draw, ','))
        {
            std::stringstream draw_stream(draw);
            draw_stream.get(); // Skip space

            std::string count_str, color_str;
            std::getline(draw_stream, count_str, ' ');
            std::getline(draw_stream, color_str);

            uint32_t count = std::stoi(count_str);
            cube_color color;

            if (color_str == "red")
            {
                color = cube_color::red;
            }
            else if (color_str == "green")
            {
                color = cube_color::green;
            }
            else if (color_str == "blue")
            {
                color = cube_color::blue;
            }

            // Part 1
            if (count > bag_contents[(uint8_t)color])
            {
                game_possible = false;
            }

            // Part 2
            min_required_cubes[(uint8_t)color] = std::max(min_required_cubes[(uint8_t)color], count);
        }
    }

    uint64_t power = min_required_cubes[(uint8_t)cube_color::red] * min_required_cubes[(uint8_t)cube_color::green] * min_required_cubes[(uint8_t)cube_color::blue];
    return std::make_pair(game_possible, power);
}

int main()
{

    std::array<uint32_t, 3> bag_contents;
    bag_contents[(uint8_t)cube_color::red] = 12;
    bag_contents[(uint8_t)cube_color::green] = 13;
    bag_contents[(uint8_t)cube_color::blue] = 14;

    std::fstream fs("input", std::fstream::in);

    std::string line;
    uint32_t game_index = 1;
    uint32_t possible_games_indices_sum = 0;
    uint64_t games_power_sum = 0;
    while (std::getline(fs, line))
    {
        auto [game_possible, game_power] = process_game(line, bag_contents);
        if (game_possible)
        {
            possible_games_indices_sum += game_index;
        }

        games_power_sum += game_power;

        game_index++;
    }

    std::cout << "possible games indices sum: " << possible_games_indices_sum << std::endl;
    std::cout << "games power sum: " << games_power_sum << std::endl;

    return 0;
};
