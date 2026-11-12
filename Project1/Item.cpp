#include "Item.h"

Item::Item(int x, int y, E_ItemType type, int value)
    : GameObject(
        x, y,
        type == E_ItemType::COIN
        ? RGB(255, 215, 0)  // 코인: 노란색
        : RGB(255, 0, 255)    // 체력: 보라색
    ),
    _type(type),
    _value(value)
{
}

void Item::Apply(Player& player)
{
    if (!_active)
        return;

    switch (_type)
    {
    case E_ItemType::COIN:
        player.AddScore(_value);
        break;

    case E_ItemType::LIFE_UP:
        player.GetHeal(_value);
        break;
    }

    _active = false;
}