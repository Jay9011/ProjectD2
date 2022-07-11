#include "Framework.h"
#include "Game.h"

#include "Engine/Core/Core.h"
#include "Engine/Manager/Device/Device.h"
#include "Engine/Manager/Scene/Scene.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Run()
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

void Game::Update()
{
}

void Game::Render()
{
}
