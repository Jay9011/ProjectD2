#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <Windows.h>
// C 런타임 헤더 파일입니다.
#include <resource.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <tchar.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
// Direct 3D
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")