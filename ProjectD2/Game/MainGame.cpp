#include "Framework.h"
#include "MainGame.h"

#include "Engine/Core/Core.h"
#include "Engine/Manager/Device.h"

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

void MainGame::Run()
{
	Update();
		
	// Device Clear
#if _DEBUG
	DEVICE->Clear
	(
		0,
		nullptr,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		0xFF787878,
		1.f,
		0
	);
#else
	DEVICE->Clear
	(
		0,
		nullptr,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		0xFFFFFF,
		1.f,
		0
	);
#endif // _DEBUG

	DEVICE->BeginScene();
	
	Render();

	DEVICE->EndScene();
	DEVICE->Present(nullptr, nullptr, nullptr, nullptr);
}

void MainGame::Update()
{
}

void MainGame::Render()
{
}
