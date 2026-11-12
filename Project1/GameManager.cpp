#include "GameManager.h"

GameManager::GameManager(int width, int height)
    : _maze(width, height), _player(1, 1),_exit(1,1)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    _objs.push_back(&_player);

    // 몬스터 생성 및 등록
    _monsterPool.emplace_back(5, 5);
    _monsterPool.emplace_back(10, 8);

    for (auto& monster : _monsterPool)
        _objs.push_back(&monster);

    // 아이템 생성 및 등록
    _itemPool.emplace_back(3, 3, E_ItemType::COIN, 100);
    _itemPool.emplace_back(7, 5, E_ItemType::LIFE_UP, 1);

    for (auto& item : _itemPool)
        _objs.push_back(&item);

    // 출구 등록
    _objs.push_back(&_exit);
}


void GameManager::RegisterObjects()
{
    // 몬스터
    for (auto& monster : _monsterPool)
    {
        _maze.SetTile(
            monster.X(), monster.Y(),
            E_MapTileType::MONSTER
        );
    }

    // 아이템
    for (auto& item : _itemPool)
    {
        _maze.SetTile(
            item.X(), item.Y(),
            E_MapTileType::ITEM
        );
    }

    // 출구
    _maze.SetTile(
        _exit.X(), _exit.Y(),
        E_MapTileType::EXIT
    );

    // 플레이어를 마지막에 등록
    _maze.SetTile(
        _player.X(), _player.Y(),
        E_MapTileType::PLAYER
    );
}


void GameManager::StartGame()
{
    _maze.Generate();

    _player.X(1);
    _player.Y(1);

    PlaceExit();

    RegisterObjects();
}

void GameManager::ProcessTurn(int dx, int dy)
{
    if (_gameOver)
        return;

    if (!_player.Move(dx, dy, _maze))
        return;

    for (auto& monster : _monsterPool)
        monster.TakeTurn(_maze, _player);

    CheckCollision();

    if (_player.GetLife() <= 0)
    {
        GameOver();
        return;
    }

    CheckItem();

    if (CheckExit())
        return;

    InvalidateRect(GetActiveWindow(), NULL, FALSE);
}

void GameManager::CheckItem()
{
    for (auto it = _itemPool.begin(); it != _itemPool.end();)
    {
        if (it->X() == _player.X() &&
            it->Y() == _player.Y())
        {
            it->Apply(_player);
            it = _itemPool.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // 렌더링 목록 재구성
    _objs.clear();

    _objs.push_back(&_player);

    for (auto& monster : _monsterPool)
        _objs.push_back(&monster);

    for (auto& item : _itemPool)
        _objs.push_back(&item);

    _objs.push_back(&_exit);
}

bool GameManager::CheckExit()
{
    if (_player.X() == _exit.X() &&
        _player.Y() == _exit.Y())
    {
        NextFloor();
        return true;
    }

    return false;
}

void GameManager::CheckCollision()
{
    for (auto it = _monsterPool.begin(); it != _monsterPool.end();)
    {
        int distance =
            abs(it->X() - _player.X()) +
            abs(it->Y() - _player.Y());

        if (distance == 1)
        {
            _player.GetDamage(1);

            GameObject* target = &(*it);

            _objs.erase(
                std::remove(_objs.begin(), _objs.end(), target),
                _objs.end()
            );

            it = _monsterPool.erase(it);
            _isColliding = false;

            break;
        }
        else
        {
            ++it;
        }
    }
}

void GameManager::NextFloor()
{
    ++_floor;

    // 미로 재생성
    _maze.Generate();

    PlaceExit();

    // 플레이어 초기화
    _player.X(1);
    _player.Y(1);

    // 기존 몬스터와 아이템 제거
    _monsterPool.clear();
    _itemPool.clear();

    // 랜덤 배치 가능한 위치 수집
    std::vector<std::pair<int, int>> positions;

    for (int y = 0; y < _maze.GetHeight(); ++y)
    {
        for (int x = 0; x < _maze.GetWidth(); ++x)
        {
            if (_maze.IsWall(x, y))
                continue;

            if (x == _player.X() && y == _player.Y())
                continue;

            if (x == _exit.X() && y == _exit.Y())
                continue;

            positions.push_back({ x, y });
        }
    }

    // 위치 하나를 랜덤으로 가져오고 목록에서 제거
    auto GetRandomPosition = [&]() -> std::pair<int, int>
        {
            int index = rand() % positions.size();

            auto pos = positions[index];

            positions.erase(positions.begin() + index);

            return pos;
        };

    // 몬스터 생성
    for (int i = 0; i < 2; ++i)
    {
        auto pos = GetRandomPosition();
        _monsterPool.emplace_back(pos.first, pos.second);
    }

    // 아이템 생성
    auto coinPos = GetRandomPosition();
    _itemPool.emplace_back(
        coinPos.first, coinPos.second,
        E_ItemType::COIN, 100
    );

    auto lifePos = GetRandomPosition();
    _itemPool.emplace_back(
        lifePos.first, lifePos.second,
        E_ItemType::LIFE_UP, 1
    );

    // 렌더링 목록 갱신
    _objs.clear();

    _objs.push_back(&_player);

    for (auto& monster : _monsterPool)
        _objs.push_back(&monster);

    for (auto& item : _itemPool)
        _objs.push_back(&item);

    _objs.push_back(&_exit);

    // 미로 타일에 오브젝트 등록
    RegisterObjects();
}

void GameManager::GameOver()
{
    MessageBox(nullptr, TEXT("GameOver"), TEXT("Debug"), MB_OK);
    _gameOver = true;
}

void GameManager::Render(HDC hdc)
{
    // 1. 맵 전체 렌더링
    for (int y = 0; y < _maze.GetHeight(); ++y)
    {
        for (int x = 0; x < _maze.GetWidth(); ++x)
        {
            RECT rect =
            {
                x * TILE_SIZE,
                y * TILE_SIZE,
                (x + 1) * TILE_SIZE,
                (y + 1) * TILE_SIZE
            };

            COLORREF color = _maze.IsWall(x, y)
                ? RGB(0, 0, 0)
                : RGB(255, 255, 255);

            HBRUSH brush = CreateSolidBrush(color);
            FillRect(hdc, &rect, brush);
            DeleteObject(brush);
        }
    }

    // 2. 오브젝트 렌더링
    for (auto obj : _objs)
    {
        RECT rect =
        {
            obj->X() * TILE_SIZE,
            obj->Y() * TILE_SIZE,
            (obj->X() + 1) * TILE_SIZE,
            (obj->Y() + 1) * TILE_SIZE
        };

        HBRUSH brush = CreateSolidBrush(obj->GetColor());
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);
    }

    _uiMgr.Render(hdc, _player.GetLife(), _player.GetMaxLife(), _player.GetScore(),_floor);
}


void GameManager::PlaceExit()
{
    std::vector<POINT> positions;

    for (int y = 1; y < _maze.GetHeight() - 1; ++y)
    {
        for (int x = 1; x < _maze.GetWidth() - 1; ++x)
        {
            if (!_maze.IsWall(x, y))
            {
                // 플레이어 시작 위치 제외
                if (x == _player.X() && y == _player.Y())
                    continue;

                positions.push_back({ x, y });
            }
        }
    }

    if (positions.empty())
        return;

    int index = rand() % positions.size();

    _exit.X(positions[index].x);
    _exit.Y(positions[index].y);
}