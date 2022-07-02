#include "../EngineFramework.h"
#include "Core.h"

Core* Core::instance = nullptr;
bool Core::isRunning = true;

bool Core::Init(HINSTANCE _hInstance)
{
	hInstance = _hInstance;

	// 기본 해상도 설정
    if(resolution.WIN_WIDTH == 0 || resolution.WIN_HEIGHT == 0)
	{
		resolution.WIN_WIDTH = 1280;
		resolution.WIN_HEIGHT = 720;
	}

    MyRegisterClass();
    if (!Create())
    {
        return false;
    }

	return true;
}

int Core::Run()
{
    while (isRunning || msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
        else
        {

        }
    }
	
	return (int)msg.wParam;
}

LRESULT Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_MOUSEMOVE:
        Core::Get()->mousePos.x = LOWORD(lParam);
        Core::Get()->mousePos.y = HIWORD(lParam);
        break;
    case WM_DESTROY:
        isRunning = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
	
    return 0;
}

bool Core::Create()
{
    hWnd = CreateWindowW(L"ProjectD2", L"ProjectD2", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    return TRUE;
}

ATOM Core::MyRegisterClass()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Core::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(CLIENT_ICO));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"ProjectD2";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(CLIENT_ICO));

    return RegisterClassExW(&wcex);
}
