#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <bitset>

struct BingoNumber
{
    int number = 0;
    bool marked = false;

    BingoNumber(int nb) : number(nb){};
    BingoNumber() {}
};

struct BingoBoard
{
    BingoNumber numbers[5][5];

    BingoNumber &get(int row, int col) { return numbers[row][col]; }
    void set(int row, int col, int number) { numbers[row][col] = BingoNumber(number); }

    bool has_won()
    {
        std::bitset<5> row_won("11111");
        bool col_won = false;
        for (int row = 0; row < 5 && !col_won; row++)
        {
            col_won = true;
            for (int col = 0; col < 5; col++)
            {
                if (!numbers[row][col].marked)
                {
                    row_won.set(row, false);
                    col_won = false;
                }
            }
        }
        return col_won || row_won.any();
    }

    int get_score()
    {
        int score = 0;
        for (int row = 0; row < 5; row++)
        {
            for (int col = 0; col < 5; col++)
            {
                if (!numbers[row][col].marked)
                {
                    score += numbers[row][col].number;
                }
            }
        }
        return score;
    }

    bool mark_number(int nb)
    {
        for (int row = 0; row < 5; row++)
        {
            for (int col = 0; col < 5; col++)
            {
                if (numbers[row][col].number == nb)
                {
                    numbers[row][col].marked = true;
                }
            }
        }
    }
};

int main()
{
    std::fstream fs("example_input", std::fstream::in);
    std::string line;

    std::vector<int> drawn_numbers;
    std::vector<BingoBoard> boards;

    std::getline(fs, line);
    auto line_stream = std::stringstream(line);

    // extract draws
    std::string draw;
    while (std::getline(line_stream, draw, ','))
    {
        drawn_numbers.push_back(std::stoi(draw));
    }

    std::getline(fs, line);

    BingoBoard bg;
    int row = 0;
    int col = 0;

    while (std::getline(fs, line))
    {
        if (line == "")
        {
            boards.push_back(bg);
            bg = BingoBoard();
            row = 0;
            col = 0;
        }
        else
        {
            line_stream = std::stringstream(line);
            while (std::getline(line_stream, line, ' '))
            {
                if (line != "")
                {
                    bg.set(row, col, std::stoi(line));
                    std::cout << std::stoi(line) << std::endl;
                    col++;
                    line_stream >> std::ws;
                }
            }
        }
        row++;
    }
    return 0;
}