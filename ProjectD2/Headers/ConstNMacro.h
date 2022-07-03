#pragma once

/*
*  Constants
*/
constexpr float PI = 3.14159265358979323846f;
#define WS_DEFAULT	( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX )

/*
*  Macros
*/
#define SAFE_DELETE(m_inst) if(m_inst != nullptr) delete m_inst; \
                              m_inst = nullptr