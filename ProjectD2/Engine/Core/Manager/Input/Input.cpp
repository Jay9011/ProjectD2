#include "Framework.h"
#include "Input.h"

Input::Input(HWND* _hWnd)
{
	m_phWnd = _hWnd;
	ZeroMemory(m_prevKeyState, sizeof(m_prevKeyState));
	ZeroMemory(m_keyState, sizeof(m_keyState));
}

Input::~Input() = default;

void Input::Update()
{
	memcpy(m_prevKeyState, m_keyState, sizeof(m_keyState));
	
	HWND hWnd = GetFocus();

	if (hWnd != nullptr)
	{
		// 윈도우 포커싱 중일때
		for (int i = 0; i < MAX_KEY_INPUT; i++)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{
				if (ISPUSHED(m_prevKeyState[i]))
					m_keyState[i] = KEY_STATE::PRESS;
				else
					m_keyState[i] = KEY_STATE::DOWN;
			}
			else
			{
				if (ISPUSHED(m_prevKeyState[i]))
					m_keyState[i] = KEY_STATE::UP;
				else
					m_keyState[i] = KEY_STATE::NONE;
			}
		}
		
		// Mouse 위치 계산
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(*m_phWnd, &pt);
		m_mousePos = { (float)pt.x, (float)pt.y };
	}
	else
	{
		// 윈도우가 포커싱되지 않았을 때
		for (int i = 0; i < MAX_KEY_INPUT; i++)
		{
			if (ISPUSHED(m_prevKeyState[i]))
				m_keyState[i] = KEY_STATE::UP;
			else if (m_prevKeyState[i] == KEY_STATE::UP)
				m_keyState[i] = KEY_STATE::NONE;
		}
	}
}