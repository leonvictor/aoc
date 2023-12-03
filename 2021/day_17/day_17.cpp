#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <bitset>
#include <limits>
#include <unordered_set>

using coord_t = int64_t;

struct vec2
{
    coord_t x, y;

    void operator+=(const vec2 &b)
    {
        x += b.x;
        y += b.y;
    }

    bool operator==(const vec2 &b) const { return x == b.x && y == b.y; }
};

template <>
struct std::hash<vec2>
{
    std::size_t operator()(const vec2 &node) const
    {
        return operator()(node.x, node.y);
    }

    std::size_t operator()(const coord_t a, const coord_t b) const
    {
        return a >= b ? a * a + a + b : a + b * b;
    }
};

std::ostream &operator<<(std::ostream &stream, const vec2 &vec)
{
    return stream << "(" << vec.x << ", " << vec.y << ")";
}

bool operator<=(const vec2 &a, const vec2 &b)
{
    return a.x <= b.x && a.y <= b.y;
}

bool operator>=(const vec2 &a, const vec2 &b) { return a.x >= b.x && a.y >= b.y; }
vec2 operator+(const vec2 &a, const vec2 &b) { return vec2{a.x + b.x, a.y + b.y}; }

struct Square
{
    vec2 top_left;
    vec2 bottom_right;

    Square(coord_t min_x, coord_t max_x, coord_t min_y, coord_t max_y) : top_left{min_x, max_y}, bottom_right{max_x, min_y} {}

    bool contains(const vec2 &point) const
    {
        return top_left.x <= point.x &&
               bottom_right.x >= point.x &&
               top_left.y >= point.y &&
               bottom_right.y <= point.y;
    }
};

struct Probe
{
    enum class State
    {
        TargetReached,
        TooShort,
        TooLong,
        WentThrough
    };

    State status;
    vec2 velocity;
    coord_t max_y = std::numeric_limits<coord_t>::min();

    std::vector<vec2> trajectory;

    Probe(vec2 velocity) : velocity(velocity)
    {
        trajectory.push_back({0, 0});
    }

    vec2 get_position() { return trajectory.back(); }

    void step()
    {
        trajectory.push_back(trajectory.back() + velocity);
        max_y = std::max(max_y, get_position().y);

        if (velocity.x > 0)
        {
            velocity.x--;
        }
        else if (velocity.x < 0)
        {
            velocity.x++;
        }
        velocity.y--;
    }

    void launch(const Square &target)
    {
        std::cout << "Launching probe w/ initial velocity " << velocity << std::endl;
        while (true)
        {
            step();
            auto pos = get_position();
            std::cout << pos << std::endl;
            if (reached(target))
            {
                std::cout << "Target hit ! Max y = " << max_y << std::endl;
                status = State::TargetReached;
                return;
            }
            else if (too_short(pos, target))
            {
                std::cout << "Too short" << std::endl;
                status = State::TooShort;
                return;
            }
            else if (went_trough(target))
            {
                std::cout << "Went trough << std::endl";
                status = State::WentThrough;
                return;
            }
            else if (too_long(target))
            {
                std::cout << "Too long" << std::endl;
                status = State::TooLong;
                return;
            }
        }
    }

    bool went_trough(const Square &target)
    {
        vec2 last_step = *(trajectory.end() - 2);
        bool c = too_long(target);
        bool a = too_short(last_step, target);
        return too_long(target) && !too_short(last_step, target);
    }

    bool reached(const Square &target)
    {
        return target.contains(get_position());
    }

    bool too_short(vec2 pos, const Square &target)
    {
        // velocity is decreasing and we're already below the target
        // /!\ this is also a definition for "too long"...
        return pos.x < target.bottom_right.x && pos.y < target.bottom_right.y;
    }

    bool too_long(const Square &target)
    {
        return get_position().x > target.bottom_right.x;
    }
};

struct Extremities
{
    uint8_t tolerance = 10;

    uint8_t count_short = 0;
    uint8_t count_long = 0;

    coord_t start_x = 0;

    bool all() { return count_long >= tolerance && count_short >= tolerance; }
    void reset()
    {
        count_short = 0;
        count_long = 0;
    }
};

struct ProbeLauncher
{
    Square target;

    ProbeLauncher(Square target) : target(target) {}

    coord_t optimize()
    {
        // P1
        coord_t max_y = std::numeric_limits<coord_t>::min();
        uint32_t success_count = 0;
        // vec2 vel_max = {target.bottom_right.x + 1, target.bottom_right.y + 1};
        vec2 velocity = {1, target.bottom_right.y};

        // std::unordered_set<vec2> accurates;
        // std::bitset<2> goalposts;
        int8_t sign = 1;
        Extremities extremities;
        // std::vector<uint8_t> extremity_counts = {0, 0};

        while (true)
        {
            auto probe = Probe(velocity);
            probe.launch(target);
            switch (probe.status)
            {
            case Probe::State::TargetReached:
                // accurates.insert(velocity);
                max_y = std::max(max_y, probe.max_y);
                success_count++;
                // std::cout << goalposts << std::endl;
                if (!extremities.all())
                {
                    velocity.x += sign;
                }
                break;

            case Probe::State::TooLong:
                extremities.count_long++;
                if (extremities.count_long >= extremities.tolerance && !extremities.all())
                {
                    velocity.x = extremities.start_x;
                    sign = -1;
                }
                // goalposts.set(1);
                // sign = -1;
                velocity.x += sign;
                break;

                // case Probe::State::WentThrough:
                // break;

            case Probe::State::TooShort:
                extremities.count_short++;
                if (extremities.count_short >= extremities.tolerance && !extremities.all())
                {
                    velocity.x = extremities.start_x;
                    sign = 1;
                }
                // goalposts.set(0);
                // extremities.count_short++;
                // sign = 1;
                velocity.x += sign;
                break;
            }

            if (extremities.all())
            {
                std::cout << "full horizon explored" << std::endl;
                // goalposts.reset();
                extremities.reset();
                extremities.start_x = velocity.x;
                velocity.y++;

                if (velocity.y >= 100)
                {
                    std::cout << success_count << " accurate initial velocities" << std::endl;
                    // std::cout << accurates.size() << " accurate initial velocities" << std::endl;
                    // 83, 126, 138, 180, 238, 411, 433
                    std::cout << "max y: " << max_y << std::endl; // 4656
                    // auto vel = accurates.back();
                    // std::cout << "last: (" << vel.x << ", " << vel.y << ")" << std::endl;
                    return max_y;
                }
            }
        };
    }
};

void test_probe(Square target, vec2 initial_vel, Probe::State expected_result)
{
    Probe probe(initial_vel);
    probe.launch(target);
    assert(probe.status == expected_result);
}

int main()
{
    Square test_target = Square(20, 30, -10, -5);

    // Test case 1
    // test_probe(test_target, {7, 2}, Probe::State::TargetReached);
    // test_probe(test_target, {6, 3}, Probe::State::TargetReached);
    // test_probe(test_target, {9, 0}, Probe::State::TargetReached);
    // test_probe(test_target, {17, -4}, Probe::State::WentThrough);
    // test_probe(test_target, {9, 0}, Probe::State::TargetReached);

    std::cout << std::endl;
    Square target = Square(241, 273, -97, -63);
    ProbeLauncher launcher(target);
    auto max_y = launcher.optimize();
    assert(max_y == 4656);
    std::cout << "max y found: " << max_y << std::endl;
    // 1128 = too low
    return 0;
};
