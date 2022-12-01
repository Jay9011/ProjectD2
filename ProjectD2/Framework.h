#pragma once

// Window ����
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#include <resource.h>
#include <Windows.h>
// ����� ����
#include <conio.h>
#include <stdlib.h>
#include <tchar.h>
#include <iostream>
#include <string>
// �޸� ����
#include <malloc.h>
#include <memory>
// �ڷᱸ�� ����
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
#include <ctime>
#include <functional>
#include <math.h>
#include <AntTweakBar.h>
#include <Lib/inc/fmod.hpp>

using namespace std;

#include "Headers/ConstNMacro.h"
#include "Headers/Types.h"
#include "Headers/Enums.h"
#include "Headers/functions.h"

#include "TweakBar/TweakBar.h"

#include "Engine/Core/Core.h"
#include "Game.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "fmod_vc.lib")