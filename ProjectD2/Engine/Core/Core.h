#pragma once
class Core
{
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
	
private:
#pragma region private Member
	HINSTANCE hInstance;
	HWND      hWnd;
	MSG 	  msg;
	
	RESOLUTION resolution;

	static bool isRunning;
	D3DXVECTOR2 mousePos;
#pragma endregion
#pragma region private Member Function
	ATOM MyRegisterClass();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool Create();
#pragma endregion

/*
*	Member
*/
public:
	

private:


/*
*  Singleton
*/
#pragma region Singleton
private:
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

