#pragma once
#include "GameObject.h"
#include "Maze.h"
#include "Player.h"

class Monster : public GameObject
{
public : 
	Monster(int x, int y);

	void TakeTurn(const Maze& maze, const Player& player);

};

