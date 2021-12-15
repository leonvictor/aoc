#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <sstream>
#include <map>
#include <set>
#include <unordered_map>
#include <chrono>

struct Node
{
    uint32_t x, y;
    uint8_t risk;

    mutable bool visited = false;
    mutable uint32_t distance = UINT32_MAX;
    mutable const Node *pPrevious = nullptr;

    bool operator==(const Node &b) const { return x == b.x && y == b.y; }
    bool operator!=(const Node &b) const { return !operator==(b); }
};

template <>
struct std::hash<Node>
{
    std::size_t operator()(const Node &node) const
    {
        return operator()(node.x, node.y);
    }

    std::size_t operator()(const uint32_t a, const uint32_t b) const
    {
        return a >= b ? a * a + a + b : a + b * b;
    }
};

bool compare_distance(const Node *a, const Node *b)
{
    return a->distance > b->distance;
}

struct Graph
{
    std::unordered_map<size_t, Node> nodes;
    uint32_t max_x = 0;
    uint32_t max_y = 0;

    void add_node(uint32_t x, uint32_t y, uint8_t risk)
    {
        nodes.emplace(std::hash<Node>{}(x, y), Node{x, y, risk});
        max_x = std::max(x, max_x);
        max_y = std::max(y, max_y);
    }

    const Node *get_node(uint32_t x, uint32_t y) const
    {
        auto &node = nodes.at(std::hash<Node>{}(x, y));
        return &node;
    }

    const Node *start() const { return get_node(0, 0); }
    const Node *end() const { return get_node(max_x, max_y); }

    std::vector<const Node *> get_neighbors(const Node *node)
    {
        // todo: ugly af
        std::vector<std::pair<uint32_t, uint32_t>> neighbor_pos;
        if (node->x > 0)
        {
            neighbor_pos.push_back({node->x - 1, node->y});
        }
        if (node->x < max_x)
        {
            neighbor_pos.push_back({node->x + 1, node->y});
        }
        if (node->y > 0)
        {
            neighbor_pos.push_back({node->x, node->y - 1});
        }
        if (node->y < max_y)
        {
            neighbor_pos.push_back({node->x, node->y + 1});
        }

        std::vector<const Node *> neighbors;
        for (auto &pos : neighbor_pos)
        {
            neighbors.push_back(&nodes.at(std::hash<Node>{}(pos.first, pos.second)));
        }
        return neighbors;
    }
};

const Node *dijkstra(Graph &graph, const Node *start, const Node *end)
{
    std::vector<const Node *> container;
    container.reserve(100);
    std::priority_queue<const Node *, std::vector<const Node *>, decltype(compare_distance) *> q(compare_distance, std::move(container));

    start->distance = 0;
    q.push(start);

    while (!q.empty())
    {
        auto u = q.top();
        q.pop();

        u->visited = true;

        for (auto &v : graph.get_neighbors(u))
        {
            if (v->visited)
                continue;

            auto alt = u->distance + v->risk;
            if (alt < v->distance)
            {
                v->distance = alt;
                v->pPrevious = u;

                if (v == end) // early out if we reached the end
                    return v;

                q.push(v);
            }
        }
    }

    return nullptr;
}

int main()
{
    std::string input_path = "input";

    std::fstream fs(input_path, std::fstream::in);
    std::string line;

    // get the sizes
    int size_y = 1;
    std::getline(fs, line);
    int size_x = line.size();
    while (std::getline(fs, line))
    {
        size_y++;
    }

    // start again
    fs = std::fstream(input_path, std::fstream::in);

    Graph graph;
    int y = 0;
    while (std::getline(fs, line))
    {
        int x = 0;
        for (auto &pos : line)
        {
            // part 2
            for (int mul_x = 0; mul_x < 5; ++mul_x)
            {
                for (int mul_y = 0; mul_y < 5; ++mul_y)
                {
                    auto risk = ((((pos - '0') + (mul_y) + (mul_x)) - 1) % 9) + 1;
                    graph.add_node((size_x * mul_x) + x, (size_y * mul_y) + y, risk);
                }
            }
            x++;
        }
        y++;
    }

    auto start = graph.start();
    auto end = graph.end();
    auto path = dijkstra(graph, start, end);

    auto current = end;
    uint32_t total_risk = 0;
    while (current != start)
    {
        total_risk += current->risk;
        current = current->pPrevious;
    }

    std::cout << "total risk: " << total_risk << std::endl;
    return 0;
};
