#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <iomanip>
#include <stack>
#include <array>

struct ALU2
{
    std::stack<int> z; // z is a number in base 26

    void process(int input, int div, int check, int sum)
    {
        if ((z.top() + check) != input)
        {
            if (div == 26)
            {
                z /= div; // 1 or 26
            }
            z *= 26;
            z += input + sum;
        }
        else
        {
            z /= div;
        }
    }
};

struct ALU
{

    // we can remove x
    int cond_process(int input, int div, int arg2, int sum)
    {
        if (((z % 26) + arg2) != input)
        {
            z /= div; // 1 or 26
            z *= 26;
            z += input + sum;
        }
        else
        {
            z /= div;
        }
    }

    // factorized version
    // int factorized_process(int input, int arg1, int arg2, int arg3)
    // {
    //     x = ((z % 26) + arg2) != input;
    //     z /= arg1; // 1 or 26
    //     z *= (25 * x) + 1;
    //     z += (input + arg3) * x;
    // }

    // raw version
    // any block only depends on 3 variable !
    // int raw_process(int input, int arg1, int arg2, int arg3)
    // {
    //     w = input;
    //     x *= 0;
    //     x += z;
    //     x %= 26;
    //     z /= arg1; // 1 or 26
    //     x += arg2; // any int
    //     x = x == w;
    //     x = x == 0;
    //     y *= 0;
    //     y += 25;
    //     y *= x;
    //     y += 1;
    //     z *= y;
    //     y *= 0;
    //     y += w;
    //     y += arg3; // positive int
    //     y *= x;
    //     z += y;
    // }
};

int main()
{
    std::fstream fs("input", std::fstream::in);
    std::string line;

    std::array<std::vector<std::string>, 14> monad;
    std::getline(fs, line);
    std::vector<std::string> instructions;
    std::vector<std::array<int, 3>> parameters;
    std::array<int, 3> param;
    instructions.push_back(line);
    size_t i = 0;
    size_t i2 = 0;
    while (std::getline(fs, line))
    {
        if (line[0] == 'i')
        {
            monad[i] = instructions;
            instructions.clear();
            i++;
        }
        instructions.push_back(line);

        if (i2 == 17)
        {
            i2 = 0;
            parameters.push_back(param);
            param = std::array<int, 3>();
        }
    }
    monad[i] = instructions;

    for (int i = 0; i < monad[0].size(); ++i)
    {
        for (auto &m : monad)
        {
            std::cout << std::setw(10);
            std::cout << m[i] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
};
