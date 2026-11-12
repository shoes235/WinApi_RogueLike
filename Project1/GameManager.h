#pragma once

#include <vector>
using std::vector;

#include <memory>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <utility>

#include "Maze.h"
#include "Player.h"
#include "Monster.h"
#include "Item.h"
#include "Exit.h"
#include "UIManager.h"


class GameManager
{
private:
	static constexpr int TILE_SIZE = 32;

	UIManager _uiMgr;

	Maze _maze;
	vector<GameObject*> _objs;

	Player _player;
	vector<Monster> _monsterPool;
	vector<Item> _itemPool;
	Exit _exit;

	bool _isColliding = false;

	int _floor;
	bool _gameOver;

public:
	GameManager(int width, int height);

	void StartGame();
	void RegisterObjects();

	void ProcessTurn(int dx, int dy);

	void CheckItem();
	bool CheckExit();
	void CheckCollision();

	void NextFloor();

	void GameOver();

	void Render(HDC hdc);

	void PlaceExit();


};

