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

#if _DEBUG
	if (GetAsyncKeyState(VK_F9))
	{
		int a = 0;
	}
#endif // _DEBUG

	if (hWnd != nullptr)
	{
		// ������ ��Ŀ�� ���϶�
		for (size_t i = 0; i < MAX_KEY_INPUT; i++)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{
				if (ISPUSHED(m_prevKeyState[i]))
				{
					m_keyState[i] = KEY_STATE::PRESS;
				}
				else
				{
					m_keyState[i] = KEY_STATE::DOWN;
				}
			}
			else
			{
				if (ISPUSHED(m_prevKeyState[i]))
				{
					m_keyState[i] = KEY_STATE::UP;
				}
				else
				{
					m_keyState[i] = KEY_STATE::NONE;
				}
			}
		}
		
		// Mouse ��ġ ���
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(*m_phWnd, &pt);
		m_mousePos = { (float)pt.x, (float)pt.y };
	}
	else
	{
		// �����찡 ��Ŀ�̵��� �ʾ��� ��
		for (size_t i = 0; i < MAX_KEY_INPUT; i++)
		{
			if (ISPUSHED(m_prevKeyState[i]))
			{
				m_keyState[i] = KEY_STATE::UP;
#if _DEBUG
				if (m_pDebugWnd != nullptr) DebugRender(L"KeyUp");
#endif // _DEBUG
			}
			else if (m_prevKeyState[i] == KEY_STATE::UP)
			{
				m_keyState[i] = KEY_STATE::NONE;
#if _DEBUG
				if (m_pDebugWnd != nullptr) DebugRender(L"KeyNone");
#endif // _DEBUG
			}
		}
	}
}