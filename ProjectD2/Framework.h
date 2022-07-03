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
#include <vector>
// Direct 3D
#include <d3d9.h>
#include <d3dx9.h>
// etc...
#include <functional>
#include <math.h>
#include "Headers/ConstNMacro.h"
#include "Headers/Types.h"
#include "Headers/Enums.h"
#include "Headers/functions.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using std::vector;
using std::list;
using std::map;
using std::make_pair;
using std::for_each;
using std::string;
using std::wstring;