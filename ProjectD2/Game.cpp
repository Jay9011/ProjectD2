#include "Framework.h"
#include "Game.h"

#include "Engine/Core/Core.h"
#include "Game/Scenes/Scenes.h"

Game::Game() :
	m_GameState(GAME_STATE::PLAY)
{
	/* === === === === ===
	*       기본 설정
	* === === === === === */
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// Backface culling 사용 안함
	
	/* === === === === ===
	*       Scene 추가
	* === === === === === */
	SCENE->Add("Test", new TestScene(this));
	SCENE->Add("Tool", new ToolScene(this));
	
	SCENE->ChangeScene("Tool");
}

Game::~Game()
{
}

void Game::Update()
{
	SCENE->Update();
}

void Game::Render()
{
	SCENE->Render();
}

void Game::FinalUpdate()
{
	SCENE->FinalUpdate();
}
