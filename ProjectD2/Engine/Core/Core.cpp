#include "Framework.h"
#include "Core.h"

#include "Engine/Manager/Device.h"
#include "Game/MainGame.h"

/*
* static 초기화
*/
Core* Core::instance = nullptr;

HINSTANCE   Core::hInstance = nullptr;
bool        Core::isRunning = true;
D3DXVECTOR2 Core::mousePos  = { 0, 0 };

Core::Core() :
    hWnd(nullptr), hMenu(nullptr), msg({})
    , resolution({ 1280, 720 })
    , bDocking(true)
    , bMenu(true)
{
    device = nullptr;
    mainGame = nullptr;
}

Core::~Core()
{
    SAFE_DELETE(mainGame);
    SAFE_DELETE(device);
    DestroyMenu(hMenu);
	DestroyWindow(hWnd);
}


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

	/*
    *  메뉴 생성
    */
	hMenu = LoadMenu(nullptr, MAKEINTRESOURCE(IDR_MENU1));

	/*
    *  해상도에 맞게 윈도우 크기 조정
    */
    DockingMenu(true);

	/*==========================
    *  Manager 초기화
    * ========================== */
    device = new Device;


    /*==========================
    *  Game Setting
    * ========================== */
    mainGame = new MainGame;
	
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
            
            mainGame->Run();
            
        }
    }
	
	return (int)msg.wParam;
}

void Core::DockingMenu(bool _bDocking)
{
    if (_bDocking)
	{
		SetMenu(hWnd, hMenu);
	}
	else
	{
		SetMenu(hWnd, nullptr);
	}
	ChangeWindowSize(resolution, _bDocking);
}

bool Core::ChangeWindowSize(UINT _width, UINT _height)
{
    return ChangeWindowSize({ _width, _height }, bMenu);
}

bool Core::ChangeWindowSize(RESOLUTION _resolution, bool _bMenu)
{
	RECT rect = { 0, 0, _resolution.WIN_WIDTH, _resolution.WIN_HEIGHT };
    if (!AdjustWindowRect(&rect, WS_DEFAULT, _bMenu))
    	    return FALSE;
	
    if (!SetWindowPos(hWnd, nullptr,
            int((float)GetSystemMetrics(SM_CXSCREEN) * 0.5f - (float)rect.right * 0.5f),
            int((float)GetSystemMetrics(SM_CYSCREEN) * 0.5f - (float)rect.bottom * 0.5f),
            rect.right - rect.left,
            rect.bottom - rect.top,
            0
        ))
        return FALSE;
	
    resolution = _resolution;
    return TRUE;
}

bool Core::Create()
{
    hWnd = CreateWindowW(L"ProjectD2", L"ProjectD2", WS_DEFAULT,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ChangeWindowSize(resolution, bMenu);
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    return TRUE;
}

ATOM Core::MyRegisterClass()
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = Core::WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(CLIENT_ICO));
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName  = nullptr;
    wcex.lpszClassName = L"ProjectD2";
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(CLIENT_ICO));

    return RegisterClassExW(&wcex);
}

/*
*  해상도 변경 이벤트 처리
*/
INT_PTR CALLBACK ResolutionDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			Core* core = Core::Get();
			core->Resolution().WIN_WIDTH  = GetDlgItemInt(hDlg, IDC_WIDTH , nullptr, FALSE);
			core->Resolution().WIN_HEIGHT = GetDlgItemInt(hDlg, IDC_HEIGHT, nullptr, FALSE);
			core->ChangeWindowSize(core->WIN_WIDTH(), core->WIN_HEIGHT());
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

/*
*  윈도우 이벤트 처리
*/
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
        case IDM_SetResolution:
        {
            DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)ResolutionDlgProc);
        }
        break;
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
        Core::mousePos.x = LOWORD(lParam);
        Core::mousePos.y = HIWORD(lParam);
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