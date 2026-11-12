#pragma once
#include <Windows.h>

class GameObject
{
protected:
	union Vector2
	{
		struct
		{
			int x; int y;
		};
		int v[2];
	};

	Vector2 _pos;
	COLORREF _cols = 0x00000000; // °ËÀº»ö

public:
	int X() const { return _pos.x; }
	void X(int x) { _pos.x = x; }

	int Y() const { return _pos.y; }
	void Y(int y) { _pos.y = y; }


public:
	GameObject(int x, int y, COLORREF col)
		: _pos{{x,y}}, _cols(col)
	{ }
	virtual ~GameObject() {}

	COLORREF GetColor() { return _cols; }

};

