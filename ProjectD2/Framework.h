#pragma once

// Window 관련
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#include <resource.h>
#include <Windows.h>
// 입출력 관련
#include <conio.h>
#include <stdlib.h>
#include <tchar.h>
#include <iostream>
#include <string>
// 메모리 관련
#include <malloc.h>
#include <memory>
// 자료구조 관련
#include <algorithm>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>
// Direct 3D
#include <d3d9.h>
#include <d3dx9.h>
// etc...
#include <cassert>
#include <cmath>
#include <functional>
#include <math.h>
#include <AntTweakBar.h>

using std::vector;
using std::list;
using std::map;
using std::unordered_map;
using std::make_pair;
using std::for_each;
using std::string;
using std::wstring;

#include "Headers/ConstNMacro.h"
#include "Headers/Types.h"
#include "Headers/Enums.h"
#include "Headers/functions.h"

#include "TweakBar/TweakBar.h"

#include "Engine/Core/Core.h"
#include "Game.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
