#include "Framework.h"
#include "Core.h"

#include "Game.h"
#include "Engine/Resource/Shader.h"

#if _DEBUG
HANDLE hConsole;
#endif // _DEBUG

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
    m_pathMgr = nullptr;
	m_sceneMgr = nullptr;
    m_textureMgr = nullptr;
    m_camera = nullptr;
    m_Game = nullptr;
}

Core::~Core()
{
#if _DEBUG
    TweakBar::Delete(); // Init에서 Device 다음에 생성됨
#endif // _DEBUG
    SAFE_DELETE(m_Game);
    SAFE_DELETE(m_camera);
	SAFE_DELETE(m_pathMgr);
	SAFE_DELETE(m_input);
	SAFE_DELETE(m_timer);
    SAFE_DELETE(m_device);
	SAFE_DELETE(m_sceneMgr);
	SAFE_DELETE(m_textureMgr);
    DestroyMenu(m_hMenu);
	DestroyWindow(m_hWnd);

    Shader::Delete();
}


bool Core::Init(HINSTANCE _hInstance)
{
	m_hInstance = _hInstance;

	/*
    * 기본 해상도 설정
    */
    if(m_resolution.width == 0 || m_resolution.height == 0)
	{
		m_resolution.width = 1280;
		m_resolution.height = 720;
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
    m_device     = new Device(&m_hWnd, &m_resolution);
    m_timer      = new Timer;
	m_input      = new Input(&m_hWnd);
    m_pathMgr    = new PathMgr(L"\\Bin\\Content\\");
	m_sceneMgr   = new SceneMgr;
	m_textureMgr = new TextureMgr;
    m_camera     = new Camera;

#if _DEBUG
    TweakBar::Create();
#endif // _DEBUG

    /* === === === === ===
    *    Game Setting
    * === === === === === */
    m_Game = new Game;
	
	return true;
}

int Core::Run()
{
#if _DEBUG
    if (AllocConsole())
    {
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
#endif // _DEBUG

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
            m_input->Update();

            /* === === === === ===
            *     Game Update
            * === === === === === */
            m_Game->Update();

            /* === === === === ===
            *  Render 전 마지막 Update
            * === === === === === */
            m_camera->Update();

            /* === === === === ===
            *	     Render
            * === === === === === */
            // Device Clear
#if _DEBUG
            DEVICE->Clear
            (
                0,
                nullptr,
                D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                0xFF787878,
                1.f,
                0
            );
#else
            DEVICE->Clear
            (
                0,
                nullptr,
                D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                0xFFFFFF,
                1.f,
                0
            );
#endif // _DEBUG
            DEVICE->BeginScene();

            /* === === === === ===
            *   최초 Render 처리
            * === === === === === */
            m_camera->Render();
			
            /* === === === === ===
            *     Game Render
            * === === === === === */
            m_Game->Render();

            /* === === === === ===
            *    Manager Render
            * === === === === === */
			// FPS 표시
			SetDlgItemText(m_hMonitorWnd, IDC_FPSTEXT, std::to_wstring(m_timer->FPS()).c_str());
			// 마우스 위치 표시
			D3DXVECTOR2 mousePos = m_input->GetMousePos();
			wstring mousePosStr = L"X : " + std::to_wstring(mousePos.x) + L" Y : " + std::to_wstring(mousePos.y);
            SetDlgItemText(m_hMonitorWnd, IDC_MOUSEPOSTXT, mousePosStr.c_str());
			// 리소스 위치 표시
			SetDlgItemText(m_hMonitorWnd, IDC_RESOURCEPATH, m_pathMgr->GetContentPath());
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

#if _DEBUG
            TweakBar::Get()->Render();
#endif // _DEBUG

            DEVICE->EndScene();
            DEVICE->Present(nullptr, nullptr, nullptr, nullptr);

			
            /* === === === === ===
            *     Final Update
            * === === === === === */
            m_Game->FinalUpdate();
        }
    }

#if _DEBUG
    FreeConsole();
#endif // _DEBUG
	
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
	
	RECT rect = { 0, 0, _resolution.width, _resolution.height};
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
            core->Resolution().width = GetDlgItemInt(hDlg, IDC_WIDTH, nullptr, FALSE);
            core->Resolution().height = GetDlgItemInt(hDlg, IDC_HEIGHT, nullptr, FALSE);
            core->ChangeWindowSize(core->WinWidth(), core->WinHeight());
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    default:
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
            timer->SetMaxFPS((float)GetDlgItemInt(hDlg, IDC_MAXFPS, nullptr, FALSE));
            SetDlgItemText(hDlg, IDC_MAXFPS, std::to_wstring(timer->GetMaxFPS()).c_str());
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    default:
        break;
    }
    return (INT_PTR)FALSE;
}

/*
*  윈도우 이벤트 처리
*/
LRESULT Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#if _DEBUG
    TweakBar::Get()->InputProc(hWnd, message, wParam, lParam);
#endif // _DEBUG

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
        BeginPaint(hWnd, &ps);
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