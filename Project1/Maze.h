#pragma once
#include <Windows.h>

#include <vector>
using std::vector;

enum class E_MapTileType
{
	NONE,
	WALL,
	PLAYER,
	MONSTER,
	ITEM,
	EXIT
};
class Maze
{
private:
    vector<vector<E_MapTileType>> _map;

    int _width;
    int _height;

public:
    Maze(int width, int height);

    void Generate();

    E_MapTileType GetTile(int x, int y) const;
    void SetTile(int x, int y, E_MapTileType type);

    bool IsWall(int x, int y) const;

    int GetWidth() const { return _width; }
    int GetHeight() const { return _height; }
};