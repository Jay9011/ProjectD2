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
		// ������ ��Ŀ�� ���϶�
		for (size_t i = 0; i < MAX_KEY_INPUT; i++)
		{
			if (GetAsyncKeyState(i) & 0x8000)
			{

			}
		}
	}
	else
	{
		// �����찡 ��Ŀ�̵��� �ʾ��� ��
	}
}
