#include "Engine/EngineFramework.h"

#if _DEBUG
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

int APIENTRY wWinMain(_In_     HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_     LPWSTR    lpCmdLine,
                      _In_     int       nCmdShow)
{
    // 메모리 릭(누수) 체크
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(205);
	
    if (!Core::Get()->Init(hInstance))
    {
        Core::Delete();
        return 0;
    }

    int result = Core::Get()->Run();

    Core::Delete();

    return result;
}