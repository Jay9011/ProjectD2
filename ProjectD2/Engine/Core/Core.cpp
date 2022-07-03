#include "Framework.h"
#include "Core.h"

#include "Engine/Manager/Device.h"
#include "Engine/Manager/Timer.h"
#include "Engine/Manager/Input.h"
#include "Game/MainGame.h"

/* === === === === ===
*    static 초기화
* === === === === === */
Core* Core::m_inst = nullptr;

HINSTANCE   Core::m_hInstance = nullptr;
HWND        Core::m_hMonitorWnd = nullptr;
bool        Core::m_isRunning = true;
D3DXVECTOR2 Core::m_mousePos  = { 0, 0 };

Core::Core() :
    m_hWnd(nullptr), m_hMenu(nullptr), m_msg({})
    , m_resolution({ 1280, 720 })
    , m_bDocking(true)
    , m_bMenu(true)
{
    m_device = nullptr;
    m_timer = nullptr;
	m_input = nullptr;
    m_mainGame = nullptr;
}

Core::~Core()
{
    SAFE_DELETE(m_mainGame);
	SAFE_DELETE(m_input);
	SAFE_DELETE(m_timer);
    SAFE_DELETE(m_device);
    DestroyMenu(m_hMenu);
	DestroyWindow(m_hWnd);
}


bool Core::Init(HINSTANCE _hInstance)
{
	m_hInstance = _hInstance;

	/*
    * 기본 해상도 설정
    */
    if(m_resolution.WIN_WIDTH == 0 || m_resolution.WIN_HEIGHT == 0)
	{
		m_resolution.WIN_WIDTH = 1280;
		m_resolution.WIN_HEIGHT = 720;
	}

	/*
    * 윈도우 생성
    */
    MyRegisterClass();
    if (!Create())
    {
        return false;
    }

	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCE(IDR_MENU1));  // 메뉴 생성
    DockingMenu(m_bDocking);  // 메뉴 장착 및 해상도에 맞게 윈도우 크기 조정

	/* === === === === ===
    *   Manager 초기화
    * === === === === === */
    m_device = new Device;
    m_timer  = new Timer;
	m_input  = new Input;


    /* === === === === ===
    *    Game Setting
    * === === === === === */
    m_mainGame = new MainGame;
	
	return true;
}

int Core::Run()
{
    while (m_isRunning || m_msg.message != WM_QUIT)
    {
        if (PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE))
        {
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
		}
        else
        {
            /* === === === === ===
            *    Manager Update
            * === === === === === */
            m_timer->Update();

            /* === === === === ===
            *     Game 진행
            * === === === === === */
            m_mainGame->Run();
			

            /* === === === === ===
            *    Manager Render
            * === === === === === */
			// FPS 표시
			SetDlgItemText(m_hMonitorWnd, IDC_FPSTEXT, std::to_wstring(m_timer->FPS()).c_str());
            // 시간 표시
#pragma region 시간 표시
            double runTime = m_timer->RunningTime();
			int hour   = (int)runTime / 3600;
			int min    = (int)(runTime - hour * 3600) / 60;
			double sec = runTime - hour * 3600 - min * 60;
			wstring hourStr = std::to_wstring(hour);
			wstring minStr  = std::to_wstring(min);
			wstring secStr  = std::to_wstring(sec);
			if (hourStr.size() == 1)
			{
				hourStr = L"0" + hourStr;
			}
			if (minStr.size() == 1)
			{
				minStr = L"0" + minStr;
			}
			if (sec < 10)
			{
				secStr = L"0" + secStr;
			}
			wstring timeStr = hourStr + L":" + minStr + L":" + secStr;
			SetDlgItemText(m_hMonitorWnd, IDC_RUNTIME, timeStr.c_str());
#pragma endregion
        }
    }
	
	return (int)m_msg.wParam;
}

void Core::DockingMenu(bool _bDocking)
{
    if (_bDocking)
	{
		SetMenu(m_hWnd, m_hMenu);
	}
	else
	{
		SetMenu(m_hWnd, nullptr);
	}
	ChangeWindowSize(m_resolution, _bDocking);
}

bool Core::ChangeWindowSize(RESOLUTION _resolution, bool _bMenu, HWND _hWnd)
{
	HWND hWnd = _hWnd;

    if(hWnd == nullptr && m_hWnd == nullptr)
		return FALSE;

	if(hWnd == nullptr)
		hWnd = m_hWnd;
	
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
	
    m_resolution = _resolution;
    return TRUE;
}

bool Core::Create()
{
    m_hWnd = CreateWindowW(L"ProjectD2", L"ProjectD2", WS_DEFAULT,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInstance, nullptr);

    if (!m_hWnd)
    {
        return FALSE;
    }

    ChangeWindowSize(m_resolution, m_bMenu);
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

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
    wcex.hInstance     = m_hInstance;
    wcex.hIcon         = LoadIcon(m_hInstance, MAKEINTRESOURCE(CLIENT_ICO));
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
            core->Resolution().WIN_WIDTH = GetDlgItemInt(hDlg, IDC_WIDTH, nullptr, FALSE);
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
* 모니터링 이벤트 처리
*/
INT_PTR CALLBACK MonitoringDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        SetDlgItemText(hDlg, IDC_MAXFPS, std::to_wstring(TIMER->GetMaxFPS()).c_str());
    }
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK)
        {
            // MaxFPS 변경
            auto timer = TIMER;
            timer->SetMaxFPS(GetDlgItemInt(hDlg, IDC_MAXFPS, nullptr, FALSE));
            SetDlgItemText(hDlg, IDC_MAXFPS, std::to_wstring(timer->GetMaxFPS()).c_str());
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
            DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)ResolutionDlgProc);
        }
            break;
        case IDM_MONITORING:
        {
            if (!IsWindow(m_hMonitorWnd))
            {
                m_hMonitorWnd = CreateDialog(m_hInstance, MAKEINTRESOURCE(IDD_MONITOR), hWnd, (DLGPROC)MonitoringDlgProc);
				ShowWindow(m_hMonitorWnd, SW_SHOW);
			}
			else
			{
                SetFocus(m_hMonitorWnd);
			}
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
        Core::m_mousePos.x = LOWORD(lParam);
        Core::m_mousePos.y = HIWORD(lParam);
        break;
    case WM_DESTROY:
        m_isRunning = false;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}