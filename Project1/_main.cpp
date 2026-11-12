#include <Windows.h>
#include "GameManager.h"

GameManager* g_gameManager = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
    {
        if (lParam & (1 << 30))
            return 0;

        if (!g_gameManager)
            break;

        switch (wParam)
        {
        case VK_UP:
            g_gameManager->ProcessTurn(0, -1);
            break;

        case VK_DOWN:
            g_gameManager->ProcessTurn(0, 1);
            break;

        case VK_LEFT:
            g_gameManager->ProcessTurn(-1, 0);
            break;

        case VK_RIGHT:
            g_gameManager->ProcessTurn(1, 0);
            break;
        }

        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);

        int width = rect.right;
        int height = rect.bottom;

        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP bitmap = CreateCompatibleBitmap(hdc, width, height);

        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);

        if (g_gameManager)
            g_gameManager->Render(memDC);

        BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

        SelectObject(memDC, oldBitmap);
        DeleteObject(bitmap);
        DeleteDC(memDC);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_ERASEBKGND:
        return 1;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MazeGame";

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(
        L"MazeGame",
        L"Turn Maze Roguelike",
        WS_OVERLAPPEDWINDOW,
        100, 100,
        800, 600,
        nullptr, nullptr,
        hInstance, nullptr
    );

    ShowWindow(hwnd, nCmdShow);

    GameManager gameManager(21, 15);

    g_gameManager = &gameManager;

    gameManager.StartGame();

    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}