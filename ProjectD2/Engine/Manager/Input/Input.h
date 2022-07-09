#pragma once
#include <bitset>

#define MAX_KEY_INPUT 256

enum class KEY_STATE
{
	NONE,
	DOWN,
	PRESS,
	UP
};

class Input
{
public:
	Input();
	virtual ~Input();
	
	void Update();
	
	bool  Down(DWORD key) { return m_keyState[key] == KEY_STATE::DOWN; };
	bool Press(DWORD key) { return m_keyState[key] == KEY_STATE::PRESS; };
	bool    Up(DWORD key) { return m_keyState[key] == KEY_STATE::UP; };
	
private:
	D3DXVECTOR2 m_mousePos;
	
	KEY_STATE m_prevKeyState[MAX_KEY_INPUT];
	KEY_STATE m_keyState[MAX_KEY_INPUT];
};