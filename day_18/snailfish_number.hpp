#include <string>
#include <iostream>
#include <fstream>
#include <variant>
#include <memory>
#include <sstream>
#include <vector>
#include <assert.h>
#include <cmath>

struct snailfish_number
{
    using regular_t = uint64_t;
    using pair_t = std::shared_ptr<snailfish_number>;
    using atomic_t = std::variant<pair_t, regular_t>;

    snailfish_number::atomic_t first;
    snailfish_number::atomic_t second;
};

snailfish_number::atomic_t add_right(snailfish_number::atomic_t &source, snailfish_number::regular_t value);
snailfish_number::atomic_t add_left(snailfish_number::atomic_t &source, snailfish_number::regular_t value);
std::shared_ptr<snailfish_number> copy(std::shared_ptr<snailfish_number> &src);

std::shared_ptr<snailfish_number> reduce(std::shared_ptr<snailfish_number> &nb);
snailfish_number::regular_t get_magnitude(std::shared_ptr<snailfish_number> &nb);

snailfish_number::pair_t operator+(snailfish_number::pair_t &a, snailfish_number::pair_t &b);

std::ostream &operator<<(std::ostream &os, const snailfish_number::atomic_t &nb);
std::ostream &operator<<(std::ostream &os, const snailfish_number &number);

struct splitter
{
    using split_result = std::pair<bool, std::shared_ptr<snailfish_number>>;
    split_result operator()(snailfish_number::pair_t &e);
    split_result operator()(snailfish_number::regular_t &e);
};

struct explosion_result
{
    bool exploded = false;
    snailfish_number::atomic_t previous;
    std::optional<snailfish_number::regular_t> left, right;
};

struct exploder
{
    uint8_t recursion_level = 0;

    explosion_result operator()(snailfish_number::pair_t &e);
    explosion_result operator()(snailfish_number::regular_t &e);
};
