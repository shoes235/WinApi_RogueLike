#pragma once
#include "Maze.h"
#include "GameObject.h"

class Player : public GameObject
{
private:
	size_t _curLiv;
	size_t _maxLiv;
	int _score;

public : 
	Player(int x, int y);

	bool Move(int dx, int dy,Maze& maze);

	void GetDamage(int v);
	void GetHeal(int v);

	void AddScore(int v);

	int GetLife() const { return _curLiv; }
	int GetMaxLife() const { return _maxLiv; }

	int GetScore() const { return _score; }



};

