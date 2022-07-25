#pragma once
// Direct 3D
#include <d3d9.h>
#include <d3dx9.h>

/*
*  Constants
*/
constexpr float PI = 3.14159265358979323846f;
#define V_LEFT  D3DXVECTOR2(-1.0f, 0.0f)
#define V_RIGHT D3DXVECTOR2(+1.0f, 0.0f)
#define V_UP    D3DXVECTOR2(0.0f, -1.0f)
#define V_DOWN  D3DXVECTOR2(0.0f, +1.0f)
#define WS_DEFAULT	( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX )

/*
*  Macros
*/
#define SAFE_DELETE(m_inst) if(m_inst != nullptr) delete m_inst; \
                              m_inst = nullptr