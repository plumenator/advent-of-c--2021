#include <iostream>
#include <set>

auto fold(const std::set<std::pair<int, int>> &points, char axis, int value)
{
    std::set<std::pair<int, int>> result;
    for (const auto &p : points)
        if (axis == 'x')
            result.insert({value - std::abs(p.first - value), p.second});
        else
            result.insert({p.first, value - std::abs(p.second - value)});

    return result;
}

void part1()
{
    int x, y;
    char a;
    std::set<std::pair<int, int>> points;
    while (std::cin >> x >> a >> y)
        points.insert({x, y});
    std::scanf("fold along %c=%d\n", &a, &x);
    std::cout << fold(points, a, x).size() << std::endl;
}

void part2()
{
    int x, y;
    char a;
    std::set<std::pair<int, int>> points;
    while (std::cin >> x >> a >> y)
        points.insert({x, y});

    int lim[2] = {INT32_MAX, INT32_MAX};
    while (std::scanf("fold along %c=%d\n", &a, &x) > 0)
    {
        points = fold(points, a, x);
        lim[(a - 'x')] = std::min(lim[(a - 'x')], x);
    }
    for (int y = 0; y < lim[1]; y++)
    {
        for (int x = 0; x < lim[0]; x++)
            std::cout << (points.count({x, y}) ? '#' : ' ');

        std::cout << std::endl;
    }
}

int main()
{
    part2();
    return 0;
}