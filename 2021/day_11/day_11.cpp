#include <string>
#include <iostream>
#include <fstream>
#include <array>

struct Octopus
{
    int energy_level = 0;
    bool has_flashed = false;
};

struct Grid
{
    std::array<std::array<Octopus, 10>, 10> octopus;
    bool all_flashed = false;

    int step()
    {
        int flash_count = 0;
        for (int x = 0; x < octopus.size(); x++)
        {
            for (int y = 0; y < octopus[x].size(); y++)
            {
                octopus[x][y].energy_level += 1;
            }
        }

        for (int x = 0; x < octopus.size(); x++)
        {
            for (int y = 0; y < octopus[x].size(); y++)
            {
                if (octopus[x][y].energy_level > 9)
                {
                    flash(x, y);
                }
            }
        }

        all_flashed = true;
        for (int x = 0; x < octopus.size(); x++)
        {
            for (int y = 0; y < octopus[x].size(); y++)
            {
                if (octopus[x][y].has_flashed)
                {
                    flash_count++;
                    octopus[x][y].energy_level = 0;
                    octopus[x][y].has_flashed = false;
                }
                else
                {
                    all_flashed = false;
                }
            }
        }

        return flash_count;
    }

    void flash(int x, int y)
    {
        if (octopus[x][y].energy_level <= 9 || octopus[x][y].has_flashed)
            return;

        octopus[x][y].has_flashed = true;

        for (int i = x - 1; i <= x + 1; i++)
        {
            for (int j = y - 1; j <= y + 1; j++)
            {
                if (j >= 0 && j < 10 && i >= 0 && i < 10 && !(i == x && j == y))
                {

                    octopus[i][j].energy_level++;
                    flash(i, j);
                }
            }
        }
    }
};

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    Grid grid;
    int line_idx = 0;
    while (std::getline(fs, line))
    {
        int col_idx = 0;
        for (auto &oc : line)
        {
            grid.octopus[line_idx][col_idx] = Octopus{oc - '0'};
            col_idx++;
        }
        line_idx++;
    }

    int flash_count = 0;
    int step = 0;
    while (!grid.all_flashed)
    {
        step++;
        flash_count += grid.step();
    }

    std::cout << "first sync at step " << step << std::endl;
    std::cout << flash_count << " flashes." << std::endl;

    return 0;
};
