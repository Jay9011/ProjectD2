#include "Framework.h"
#include "Game.h"

#include "Engine/Core/Core.h"
#include "Engine/Object/Effect.h"
#include "Engine/Component/Animator/Animator.h"

#include "Game/Scenes/Scenes.h"

bool Game::m_isDbgRendering = true;

Game::Game() :
	m_GameState(GAME_STATE::PLAY)
    , playerObservable(new Observable<Player>)
{
	/* === === === === ===
	*       �⺻ ����
	* === === === === === */
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// Backface culling ��� ����
	CAMERA->SetOffset({ WIN_CENTER_X, WIN_HEIGHT * 0.6f });	// Camera Offset ����

    /* === === === === ===
	*     ���� �߰�
	* === === === === ===*/
	SOUND->AddSound("BGM", "BGM.ogg", true);
	SOUND->AddSound("EnemyAttack", "EnemyAttack.wav");
	SOUND->AddSound("EnemyExplosion", "EnemyExplosion.wav");
	

	/* === === === === ===
	*       Scene �߰�
	* === === === === === */
	m_loadingScene = (LoadingScene*)SCENE->Add("Loading", new LoadingScene(this));
	SCENE->Add("Intro", new TestScene2(this));
	SCENE->Add("Test", new TestScene(this));
	
	SCENE->ChangeScene("Intro");
}

Game::~Game()
{
	SAFE_DELETE(playerObservable);
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

void Game::NextScene(const string& sceneName)
{
	m_loadingScene->SetNextScene(sceneName);
	SCENE->ChangeScene("Loading");
}
