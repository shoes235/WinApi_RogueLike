
#include "Maze.h"
#include <random>
#include <algorithm>

// 생성자
Maze::Maze(int width, int height)
    : _width(width), _height(height)
{
    _map.resize(
        height,
        vector<E_MapTileType>(width, E_MapTileType::WALL)
    );
}

// 랜덤 미로 생성
void Maze::Generate()
{
    for (auto& row : _map)
        std::fill(row.begin(), row.end(), E_MapTileType::WALL);

    static std::mt19937 rng(std::random_device{}());

    // 3칸 간격으로 이동
    const int dx[] = { 0, 0, -3, 3 };
    const int dy[] = { -3, 3, 0, 0 };

    vector<POINT> stack;

    // 2x2 방 생성
    auto CarveRoom = [&](int x, int y)
        {
            for (int i = 0; i < 2; ++i)
            {
                for (int j = 0; j < 2; ++j)
                {
                    _map[y + i][x + j] = E_MapTileType::NONE;
                }
            }
        };

    POINT start = { 1, 1 };

    CarveRoom(start.x, start.y);
    stack.push_back(start);

    while (!stack.empty())
    {
        POINT current = stack.back();

        vector<int> directions = { 0, 1, 2, 3 };
        std::shuffle(directions.begin(), directions.end(), rng);

        bool moved = false;

        for (int dir : directions)
        {
            int nx = current.x + dx[dir];
            int ny = current.y + dy[dir];

            // 2x2 방이 외곽 벽을 침범하지 않도록 검사
            if (nx <= 0 || nx + 1 >= _width - 1 ||
                ny <= 0 || ny + 1 >= _height - 1)
                continue;

            // 이미 방문한 방인지 검사
            if (_map[ny][nx] != E_MapTileType::WALL)
                continue;

            // 연결 통로 생성
            if (dx[dir] == 3)
            {
                CarveRoom(current.x + 2, current.y);
            }
            else if (dx[dir] == -3)
            {
                CarveRoom(current.x - 1, current.y);
            }
            else if (dy[dir] == 3)
            {
                CarveRoom(current.x, current.y + 2);
            }
            else if (dy[dir] == -3)
            {
                CarveRoom(current.x, current.y - 1);
            }

            // 다음 방 생성
            CarveRoom(nx, ny);

            stack.push_back({ nx, ny });

            moved = true;
            break;
        }

        if (!moved)
            stack.pop_back();
    }
}

// 타일 데이터 가져오기
E_MapTileType Maze::GetTile(int x, int y) const
{
    if (x < 0 || x >= _width ||
        y < 0 || y >= _height)
        return E_MapTileType::WALL;

    return _map[y][x];
}

// 타일 데이터 변경
void Maze::SetTile(int x, int y, E_MapTileType type)
{
    if (x < 0 || x >= _width ||
        y < 0 || y >= _height)
        return;

    _map[y][x] = type;
}

// 벽 검사
bool Maze::IsWall(int x, int y) const
{
    return GetTile(x, y) == E_MapTileType::WALL;
}
