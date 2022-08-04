#include "Framework.h"
#include "Game.h"

#include "Engine/Core/Core.h"
#include "Game/Scenes/Scenes.h"

bool Game::m_isDbgRendering = true;

Game::Game() :
	m_GameState(GAME_STATE::PLAY)
{
	/* === === === === ===
	*       기본 설정
	* === === === === === */
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// Backface culling 사용 안함
	CAMERA->SetOffset({ WIN_CENTER_X, WIN_HEIGHT * 0.6f });	// Camera Offset 설정
	
	/* === === === === ===
	*       Scene 추가
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
