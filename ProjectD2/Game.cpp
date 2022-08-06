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
	*     ����Ʈ �߰�
	* === === === === === */
	m_targetSFX = new Effect(L"SFX\\etc\\Target.png", 6, 1, ANIM_PLAY_TYPE::LOOP);

    /* === === === === ===
	*     ���� �߰�
	* === === === === ===*/
	SOUND->AddSound("BGM", "BGM.ogg", true);
	SOUND->AddSound("EnemyAttack", "EnemyAttack.wav");
	SOUND->AddSound("EnemyExplosion", "EnemyExplosion.wav");
	

	/* === === === === ===
	*       Scene �߰�
	* === === === === === */
	SCENE->Add("Test", new TestScene(this));
	
	SCENE->ChangeScene("Test");

}

Game::~Game()
{
	SAFE_DELETE(playerObservable);
	delete m_targetSFX;
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

		/*
		* Effect Update
		*/
		m_targetSFX->Update();
	}
}

void Game::Render()
{
	SCENE->Render();
    
	// Effect Render
    m_targetSFX->Render();

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