#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#include <Windows.h>
#include <resource.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <functional>
#include <math.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
// Direct 3D
#include <d3d9.h>
#include <d3dx9.h>

// etc...
#include "Headers/Enums.h"
#include "Headers/Types.h"
#include "Headers/functions.h"
#include "Headers/Macro.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using std::vector;
using std::list;
using std::map;
using std::make_pair;
using std::for_each;
using std::string;
using std::wstring;

constexpr float PI = 3.14159265358979323846f;

#define WS_DEFAULT	( WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX )