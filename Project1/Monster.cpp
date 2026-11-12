#include "Monster.h"
#include <queue>
#include <vector>

Monster::Monster(int x, int y)
    : GameObject(x, y, RGB(255,0,0))
{
}

void Monster::TakeTurn(const Maze& maze, const Player& player)
{
    using Pos = std::pair<int, int>;

    int width = maze.GetWidth();
    int height = maze.GetHeight();

    Pos start = { X(), Y() };
    Pos playerPos = { player.X(), player.Y() };

    const int dx[] = { 0, 0, -1, 1 };
    const int dy[] = { -1, 1, 0, 0 };

    std::queue<Pos> queue;

    std::vector<std::vector<bool>> visited(
        height, std::vector<bool>(width, false)
    );

    std::vector<std::vector<Pos>> parent(
        height, std::vector<Pos>(width, { -1, -1 })
    );

    queue.push(start);
    visited[start.second][start.first] = true;

    Pos destination = { -1, -1 };

    while (!queue.empty())
    {
        Pos current = queue.front();
        queue.pop();

        // 플레이어와 인접한 칸이면 이동 종료
        if (abs(current.first - playerPos.first) +
            abs(current.second - playerPos.second) == 1)
        {
            destination = current;
            break;
        }

        for (int i = 0; i < 4; ++i)
        {
            int nx = current.first + dx[i];
            int ny = current.second + dy[i];

            if (nx < 0 || ny < 0 || nx >= width || ny >= height)
                continue;

            if (visited[ny][nx] || maze.IsWall(nx, ny))
                continue;

            visited[ny][nx] = true;
            parent[ny][nx] = current;

            queue.push({ nx, ny });
        }
    }

    if (destination.first == -1)
        return;

    Pos next = destination;

    while (parent[next.second][next.first] != start)
    {
        next = parent[next.second][next.first];

        if (next.first == -1)
            return;
    }

    X(next.first);
    Y(next.second);
}