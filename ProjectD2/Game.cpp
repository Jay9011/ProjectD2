#include "Framework.h"
#include "Game.h"

#include "Engine/Core/Core.h"
#include "Game/Scenes/Scenes.h"

Game::Game() :
	m_GameState(GAME_STATE::PLAY)
{
	/* === === === === ===
	*       �⺻ ����
	* === === === === === */
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// Backface culling ��� ����
	
	/* === === === === ===
	*       Scene �߰�
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
