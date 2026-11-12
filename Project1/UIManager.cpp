#include "UIManager.h"
#include <string>


void UIManager::Render(HDC hdc, int hp, int maxHp, int score, int floor)
{
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 255, 255));

    wchar_t buffer[100];

    swprintf_s(buffer, L"HP : %d / %d", hp, maxHp);
    TextOut(hdc, 20, 450 + 20, buffer, lstrlen(buffer));

    swprintf_s(buffer, L"SCORE : %d", score);
    TextOut(hdc, 20, 450 + 40, buffer, lstrlen(buffer));

    swprintf_s(buffer, L"FLOOR : %d", floor);
    TextOut(hdc, 20, 450 + 60, buffer, lstrlen(buffer));
}