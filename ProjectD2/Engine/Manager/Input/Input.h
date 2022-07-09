#pragma once
#include <bitset>

#define MAX_KEY_INPUT 256
#define ISPUSHED(state) ((int)state & 0x000E)

/*
* Note...
* 이전 프레임에 키가 눌려있는지 확인하려면
* 0번 비트를 제외하고 확인하면 된다.
*/
enum class KEY_STATE
{
	NONE  = 0x0000,	// 0000 0000
	UP    = 0x0001,	// 0000 0001
	DOWN  = 0x0002, // 0000 0010
	PRESS = 0x0004, // 0000 0100
};

class Input
{
public:
	Input(HWND* _hWnd);
	virtual ~Input();
	
	void Update();
	
	bool  Down(DWORD _key) const { return m_keyState[_key] == KEY_STATE::DOWN; };
	bool Press(DWORD _key) const { return m_keyState[_key] == KEY_STATE::PRESS; };
	bool    Up(DWORD _key) const { return m_keyState[_key] == KEY_STATE::UP; };
	
	const D3DXVECTOR2& GetMousePos() const { return m_mousePos; };

private:
	HWND* m_phWnd;
	D3DXVECTOR2 m_mousePos;
	
	KEY_STATE m_prevKeyState[MAX_KEY_INPUT];
	KEY_STATE m_keyState[MAX_KEY_INPUT];

#pragma region Debug Render
#if _DEBUG
public:
	HWND* m_pDebugWnd;
	int m_nDebugIDDlgItem;
	void DebugSetting(HWND* _hWnd, int _nIDDlgItem)
	{
		m_pDebugWnd = _hWnd;
		m_nDebugIDDlgItem = _nIDDlgItem;
	}
	void DebugRender(const wstring& _str)
	{
		SetDlgItemText(*m_pDebugWnd, m_nDebugIDDlgItem, _str.c_str());
	}
#endif // _DEBUG
#pragma endregion


};