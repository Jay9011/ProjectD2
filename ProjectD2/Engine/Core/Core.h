#pragma once
#define DEVICE	Core::Get()->GetDevice()->Get()
#define TIMER	Core::Get()->GetTimer()
class MainGame;
class Device;
class Timer;
class Core
{
/*
*	Managers
*/
public:
	Device* GetDevice() const { return m_device; }
	Timer*  GetTimer()  const { return m_timer; }
	Input*  GetInput()  const { return m_input; }

private:
	Device*   m_device;
	Timer*    m_timer;
	Input*    m_input;
	MainGame* m_mainGame;

/*
*	Basic functions
*/
public:
	bool Init(HINSTANCE _hInstance);
	int  Run();
	
	RESOLUTION& Resolution()   { return m_resolution; }
	UINT&       WIN_WIDTH()    { return m_resolution.WIN_WIDTH; }
	UINT&       WIN_HEIGHT()   { return m_resolution.WIN_HEIGHT; }
	float       WIN_CENTER_X() { return m_resolution.WIN_WIDTH * .5f; }
	float       WIN_CENTER_Y() { return m_resolution.WIN_HEIGHT * .5f; }

	void DockingMenu(bool _bDocking);
	bool ChangeWindowSize(RESOLUTION _resolution, bool _bMenu, HWND _hWnd = nullptr);
#pragma region ChangeWindowSizeOverloading
	bool ChangeWindowSize(UINT _width, UINT _height, bool _bMenu, HWND _hWnd = nullptr) { return ChangeWindowSize({ _width, _height }, _bMenu, _hWnd); }
	bool ChangeWindowSize(UINT _width, UINT _height) { return ChangeWindowSize({ _width, _height }, m_bMenu); }
#pragma endregion

	HWND GetHWND() { return m_hWnd; }
	
#pragma region private Member
private:
	static HINSTANCE m_hInstance;
	HWND  m_hWnd;
	static HWND m_hMonitorWnd;
	HMENU m_hMenu;
	MSG   m_msg;
	
	RESOLUTION m_resolution;
	bool       m_bDocking;
	bool       m_bMenu;

	static bool m_isRunning;
	static D3DXVECTOR2 m_mousePos;
#pragma endregion
#pragma region private Member Function
private:
	ATOM MyRegisterClass();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool Create();
#pragma endregion

/*
*  Singleton
*/
#pragma region Singleton
private:
	Core();
	virtual ~Core();
	
	static Core* m_inst;

public:
	static Core* Get()
	{
		if (m_inst == nullptr)
			m_inst = new Core;

		return m_inst;
	}
	static void  Delete() { SAFE_DELETE(m_inst); }
#pragma endregion
};

