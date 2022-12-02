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
    BingoNumber() = default;
};

struct BingoBoard
{
    BingoNumber numbers[5][5];

    bool m_won = false;
    bool m_just_won = false;

    BingoNumber &get(int row, int col) { return numbers[row][col]; }
    void set(int row, int col, int number) { numbers[row][col] = BingoNumber(number); }

    bool already_won() { return m_won; }

    bool just_won()
    {
        if (m_just_won)
        {
            m_just_won = false;
            return true;
        }
        return false;
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

    void mark_number(int nb)
    {
        for (int row = 0; row < 5; row++)
        {
            for (int col = 0; col < 5; col++)
            {
                if (numbers[row][col].number == nb)
                {
                    numbers[row][col].marked = true;
                    check_win_condition(row, col);
                }
            }
        }
    }

    void check_win_condition(int row, int col)
    {
        bool row_win = true, col_win = true;
        for (int i = 0; i < 5; i++)
        {
            row_win = row_win && numbers[row][i].marked;
            col_win = col_win && numbers[i][col].marked;
        }
        if (row_win || col_win)
        {
            m_won = true;
            m_just_won = true;
        }
    }
};

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    std::vector<int> drawn_numbers;
    std::vector<BingoBoard> boards;

    std::getline(fs, line);
    auto line_stream = std::stringstream(line);

    // extract draws
    std::string string_draw;
    while (std::getline(line_stream, string_draw, ','))
    {
        drawn_numbers.push_back(std::stoi(string_draw));
    }

    std::getline(fs, line);

    BingoBoard bg;
    int row = 0;
    int col = 0;

    while (std::getline(fs, line))
    {
        col = 0;

        if (line == "")
        {
            boards.push_back(bg);
            bg = BingoBoard();
            row = 0;
        }

        else
        {
            line_stream = std::stringstream(line);
            while (std::getline(line_stream, line, ' '))
            {
                if (line != "")
                {
                    bg.set(row, col, std::stoi(line));
                    line_stream >> std::ws;

                    col++;
                }
            }
            row++;
        }
    }
    boards.push_back(bg);

    bool winner_found = false;
    for (auto draw = drawn_numbers.begin(); draw != drawn_numbers.end() && !winner_found; draw++)
    {
        for (auto bingoboard = boards.begin(); bingoboard != boards.end() && !winner_found; bingoboard++)
        {
            bingoboard->mark_number(*draw);
            if (bingoboard->just_won())
            {
                std::cout << "winner found with a score of: " << bingoboard->get_score() * (*draw) << std::endl;
                winner_found = true;
            }
        }
    }
    return 0;
}