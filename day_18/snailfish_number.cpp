#include <string>
#include <iostream>
#include <fstream>
#include <variant>
#include <memory>
#include <sstream>
#include <vector>
#include <assert.h>
#include <cmath>
#include <optional>

#include "snailfish_number.hpp"

explosion_result exploder::operator()(snailfish_number::pair_t &e)
{
    recursion_level++;

    if (recursion_level > 4)
    {
        explosion_result result;
        result.exploded = true;
        result.previous = (snailfish_number::regular_t)0;
        result.left = std::get<snailfish_number::regular_t>(e->first);
        result.right = std::get<snailfish_number::regular_t>(e->second);

        return result;
    }

    if (auto result = std::visit(*this, e->first); result.exploded)
    {
        --recursion_level;
        e->first = result.previous;
        if (result.right.has_value())
        {
            e->second = add_left(e->second, result.right.value());
            result.right.reset();
        }

        result.previous = e;
        return result;
    }
    else if (auto result = std::visit(*this, e->second); result.exploded)
    {
        --recursion_level;
        e->second = result.previous;
        if (result.left.has_value())
        {
            e->first = add_right(e->first, result.left.value());
            result.left.reset();
        }

        result.previous = e;
        return result;
    }
    else
    {
        --recursion_level;
        return {false, e, std::nullopt, std::nullopt};
    }
}

explosion_result exploder::operator()(snailfish_number::regular_t &e)
{
    return {false, e, std::nullopt, std::nullopt};
}

snailfish_number::atomic_t add_right(snailfish_number::atomic_t &source, snailfish_number::regular_t value)
{
    struct add_right_visitor
    {
        snailfish_number::regular_t value;
        snailfish_number::atomic_t operator()(snailfish_number::pair_t &e)
        {
            e->second = std::visit(*this, e->second);
            return e;
        }
        snailfish_number::atomic_t operator()(snailfish_number::regular_t &e)
        {
            return e + value;
        }
    };

    return std::visit(add_right_visitor{value}, source);
}

snailfish_number::atomic_t add_left(snailfish_number::atomic_t &source, snailfish_number::regular_t value)
{
    struct add_left_visitor
    {
        snailfish_number::regular_t value;
        snailfish_number::atomic_t operator()(snailfish_number::pair_t &e)
        {
            e->first = std::visit(*this, e->first);
            return e;
        }
        snailfish_number::atomic_t operator()(snailfish_number::regular_t &e)
        {
            return e + value;
        }
    };

    return std::visit(add_left_visitor{value}, source);
}

snailfish_number::pair_t operator+(snailfish_number::pair_t &a, snailfish_number::pair_t &b)
{
    auto result = std::make_shared<snailfish_number>();
    result->first = copy(a);
    result->second = copy(b);
    return reduce(result);
}

std::ostream &operator<<(std::ostream &os, const snailfish_number::atomic_t &nb)
{
    struct io_visit
    {
        std::ostream &os;

        io_visit(std::ostream &os) : os{os} {};

        std::ostream &operator()(const snailfish_number::pair_t &ptr) { return os << *ptr.get(); }
        std::ostream &operator()(const snailfish_number::regular_t &number) { return os << number; }
    };

    return std::visit(io_visit{os}, nb);
}

std::ostream &operator<<(std::ostream &os, const snailfish_number &number)
{
    return os << "[" << number.first << ", " << number.second << "]";
}

splitter::split_result splitter::operator()(snailfish_number::pair_t &e)
{
    if (auto result = std::visit(*this, e->first); result.first)
    {
        if (result.second != nullptr)
        {
            e->first = result.second;
            result.second.reset();
        }
        return result;
    }
    else if (auto result = std::visit(*this, e->second); result.first)
    {
        if (result.second != nullptr)
        {
            e->second = result.second;
            result.second.reset();
        }

        return result;
    }
    else
    {
        return {false, nullptr};
    }
}

splitter::split_result splitter::operator()(snailfish_number::regular_t &e)
{
    if (e < 10)
        return {false, nullptr};

    auto splitted = std::make_shared<snailfish_number>();
    splitted->first = (snailfish_number::regular_t)std::floor((float)e / 2);
    splitted->second = (snailfish_number::regular_t)std::ceil((float)e / 2);

    return {true, splitted};
}

std::shared_ptr<snailfish_number> reduce(std::shared_ptr<snailfish_number> &nb)
{
    auto var = snailfish_number::atomic_t(nb);
    bool done = false;
    while (!done)
    {
        done = false;
        if (auto result = std::visit(exploder{}, var); result.exploded)
            continue;

        if (splitter::split_result result = std::visit(splitter{}, var); result.first)
            continue;

        done = true;
    }
    return std::get<std::shared_ptr<snailfish_number>>(var);
}

snailfish_number::regular_t get_magnitude(std::shared_ptr<snailfish_number> &nb)
{
    struct magnitude_counter
    {
        snailfish_number::regular_t operator()(snailfish_number::pair_t &e) { return 3 * std::visit(*this, e->first) + 2 * std::visit(*this, e->second); };
        snailfish_number::regular_t operator()(snailfish_number::regular_t &e) { return e; };
    };

    auto var = snailfish_number::atomic_t(nb);
    return std::visit(magnitude_counter{}, var);
}

std::shared_ptr<snailfish_number> copy(std::shared_ptr<snailfish_number> &src)
{
    struct copier
    {
        snailfish_number::atomic_t operator()(const snailfish_number::pair_t &e)
        {
            auto copy = std::make_shared<snailfish_number>();
            copy->first = std::visit(*this, e->first);
            copy->second = std::visit(*this, e->second);
            return copy;
        }

        snailfish_number::atomic_t operator()(const snailfish_number::regular_t &e) { return e; }
    };

    auto var = snailfish_number::atomic_t{src};
    var = std::visit(copier{}, var);
    return std::get<snailfish_number::pair_t>(var);
}
