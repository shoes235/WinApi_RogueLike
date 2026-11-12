#pragma once
#include "Player.h"
#include "GameObject.h"

enum class E_ItemType
{
	COIN,
	LIFE_UP
};


class Item : public GameObject
{
private:
	E_ItemType _type;
	int _value;
	bool _active = true;

public:
	Item(int x, int y, E_ItemType type, int v);

	bool IsActive() const { return _active; }
	void Deactivate() { _active = false; }

	void Apply(Player& player);

};

