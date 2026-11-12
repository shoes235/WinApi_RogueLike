#include "Player.h"

Player::Player(int x, int y)
    : GameObject(x, y, RGB(0,0,255)),
    _curLiv(3),_maxLiv(3),_score(0)
{
}

bool Player::Move(int dx, int dy, Maze& maze)
{
    int nx = X() + dx;
    int ny = Y() + dy;

    if (abs(dx) + abs(dy) != 1)
        return false;

    if (maze.IsWall(nx, ny))
        return false;

    X(nx);
    Y(ny);

    maze.SetTile(X(), Y(), E_MapTileType::PLAYER);

    return true;
}

void Player::GetDamage(int v)
{
    if (_curLiv - v > 0)
        _curLiv -= v;
    else
        _curLiv = 0;
}

void Player::GetHeal(int v)
{
    if (_curLiv + v < _maxLiv)
        _curLiv += v;
    else
        _curLiv = _maxLiv;
}

void Player::AddScore(int v)
{
    _score += v;
}