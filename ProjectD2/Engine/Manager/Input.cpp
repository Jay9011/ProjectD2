#include "Framework.h"
#include "Input.h"

Input::Input()
{
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
		for (size_t i = 0; i < MAX_KEY_INPUT; i++)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{

			}
		}
	}
	else
	{
		// 윈도우가 포커싱되지 않았을 때
	}
}
