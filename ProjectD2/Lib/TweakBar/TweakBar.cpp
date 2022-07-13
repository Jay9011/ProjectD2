#include "Framework.h"
#include "AntTweakBar.h"
#include "TweakBar.h"

TweakBar* TweakBar::instance = nullptr;

TweakBar * TweakBar::Get()
{
	return instance;
}

void TweakBar::Create()
{
	if (instance == nullptr)
		instance = new TweakBar;
}

void TweakBar::Delete()
{
	delete instance;
}

void TweakBar::Start()
{
	isDraw = true;

	BOOL result = TwInit(TW_DIRECT3D9, DEVICE);

	bar = TwNewBar("TweakBar");
	TwDefine("TweakBar color='0 128 255' alpha = 128");
	TwDefine("TweakBar size='250 500'");
	TwDefine("TweakBar position='1020 50'");
	TwDefine("TweakBar valueswidth=120");
	TwDefine("TweakBar movable=false");
}

void TweakBar::InputProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TwEventWin(wnd, msg, wParam, lParam);
}

void TweakBar::Render()
{
	TwDraw();
}

TweakBar::TweakBar()
{
	Start();
}

TweakBar::~TweakBar()
{
	TwTerminate();
}
