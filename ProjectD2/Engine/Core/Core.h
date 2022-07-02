#pragma once
class Device;
class MainGame;
class Core
{
/*
*	Members
*/
public:
	Device* GetDevice() { return device; }

private:
	MainGame* mainGame;
	Device*   device;

/*
*	Basic functions
*/
public:
	bool Init(HINSTANCE _hInstance);
	int  Run();
	
	RESOLUTION& Resolution()   { return resolution; }
	UINT&       WIN_WIDTH()    { return resolution.WIN_WIDTH; }
	UINT&       WIN_HEIGHT()   { return resolution.WIN_HEIGHT; }
	float       WIN_CENTER_X() { return resolution.WIN_WIDTH * .5f; }
	float       WIN_CENTER_Y() { return resolution.WIN_HEIGHT * .5f; }

	void DockingMenu(bool _bDocking);
	bool ChangeWindowSize(UINT _width, UINT _height);
	bool ChangeWindowSize(RESOLUTION _resolution, bool _bMenu);

	HWND GetHWND() { return hWnd; }
	
#pragma region private Member
private:
	static HINSTANCE hInstance;
	HWND      hWnd;
	HMENU     hMenu;
	MSG 	  msg;
	
	RESOLUTION resolution;
	bool       bDocking;
	bool       bMenu;

	static bool isRunning;
	static D3DXVECTOR2 mousePos;
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
	
	static Core* instance;

public:
	static Core* Get()
	{
		if (instance == nullptr)
			instance = new Core;

		return instance;
	}
	static void  Delete() { SAFE_DELETE(instance); }
#pragma endregion
};

