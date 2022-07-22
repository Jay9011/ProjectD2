#include "Framework.h"
#include "Game.h"

#include "Engine/Core/Core.h"
#include "Game/Scenes/Scenes.h"

Game::Game() :
	m_GameState(GAME_STATE::PLAY)
	, m_isDbgRendering(false)
{
	/* === === === === ===
	*       �⺻ ����
	* === === === === === */
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// Backface culling ��� ����
	
	/* === === === === ===
	*       Scene �߰�
	* === === === === === */
	SCENE->Add("Test", new TestScene(this));
	
	SCENE->ChangeScene("Test");
}

Game::~Game()
{
}

void Game::Update()
{
	if (KEYDOWN(VK_F9))
	{
		ChangeDbgRendering();
	}
	
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
