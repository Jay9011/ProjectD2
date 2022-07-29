#pragma once
#define DEVICE	Core::Get()->GetDevice()->Get()
#define TIMER	Core::Get()->GetTimer()
#define PATH	Core::Get()->GetPathMgr()
#define SCENE	Core::Get()->GetSceneMgr()
#define TEXTURE Core::Get()->GetTextureMgr()
#define WIN_RESOL	Core::Get()->Resolution()
#define WIN_WIDTH	Core::Get()->Resolution().width
#define WIN_HEIGHT	Core::Get()->Resolution().height
#define WIN_CENTER_X	Core::Get()->WinCenterX()
#define WIN_CENTER_Y	Core::Get()->WinCenterY()

#include "Manager/Device/Device.h"
#include "Manager/Timer/Timer.h"
#include "Manager/Input/Input.h"
#include "Manager/Path/PathMgr.h"
#include "Manager/Scene/SceneMgr.h"
#include "Manager/Texture/TextureMgr.h"
#include "Manager/Camera/Camera.h"

#if _DEBUG
extern HANDLE hConsole;
#endif // _DEBUG

class Game;
class Core
{
/*
*	Managers
*/
public:
	Device*     GetDevice() const     { return m_device; }
	Timer*      GetTimer() const      { return m_timer; }
	Input*      GetInput() const      { return m_input; }
	PathMgr*    GetPathMgr() const    { return m_pathMgr; }
	SceneMgr*   GetSceneMgr() const   { return m_sceneMgr; }
	TextureMgr* GetTextureMgr() const { return m_textureMgr; }
    Camera*     GetCamera() const     { return m_camera; }

private:
	Device*     m_device;
	Timer*      m_timer;
	Input*      m_input;
	PathMgr*    m_pathMgr;
	SceneMgr*   m_sceneMgr;
	TextureMgr* m_textureMgr;
	Camera*		m_camera;
	Game* m_Game;

/*
*	Basic functions
*/
public:
	bool Init(HINSTANCE _hInstance);
	int  Run();
	
	RESOLUTION& Resolution() { return m_resolution; }
	UINT&       WinWidth()   { return m_resolution.width; }
	UINT&       WinHeight()  { return m_resolution.height; }
	float       WinCenterX() { return (float)m_resolution.width * .5f; }
	float       WinCenterY() { return (float)m_resolution.height * .5f; }

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

