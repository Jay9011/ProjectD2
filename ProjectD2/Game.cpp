#include "Framework.h"
#include "Game.h"

#include "Engine/Core/Core.h"
#include "Engine/Manager/Device/Device.h"

#include "Game/Scenes/TestScene.h"

Game::Game() :
	m_GameState(GAME_STATE::PLAY)
{
	m_SceneMgr = new SceneMgr;
	
	m_SceneMgr->Add("Test", new TestScene(this));
	
	m_SceneMgr->ChangeScene("Test");
}

Game::~Game()
{
	SAFE_DELETE(m_SceneMgr);
}

void Game::Update()
{
	m_SceneMgr->Update();
}

void Game::Render()
{
	m_SceneMgr->Render();
}

void Game::Run()
{
	/* === === === === ===
	*	     Update
	* === === === === === */
	Update();
		
	/* === === === === ===
	*	     Render
	* === === === === === */
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