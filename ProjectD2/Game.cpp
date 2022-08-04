#include "Framework.h"
#include "Game.h"

#include "Engine/Core/Core.h"
#include "Game/Scenes/Scenes.h"

bool Game::m_isDbgRendering = true;

Game::Game() :
	m_GameState(GAME_STATE::PLAY)
{
	/* === === === === ===
	*       �⺻ ����
	* === === === === === */
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// Backface culling ��� ����
	CAMERA->SetOffset({ WIN_CENTER_X, WIN_HEIGHT * 0.6f });	// Camera Offset ����
	
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
	if (KEYDOWN(VK_F7))
	{
		ChangeGameState();
	}
	if (KEYDOWN(VK_F9))
	{
		ChangeDbgRendering();
	}
	
	if (m_GameState == GAME_STATE::PLAY)
	{
		SCENE->Update();
	}
}

void Game::Render()
{
	SCENE->Render();
	if (m_isDbgRendering)
	{
        CAMERA->RenderCameraRect();
		TweakBar::Get()->Render();
	}
}

void Game::FinalUpdate()
{
	if (m_GameState == GAME_STATE::PLAY)
	{
		SCENE->FinalUpdate();
	}
}
