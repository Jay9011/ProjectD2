#include "Framework.h"
#include "Game.h"

#include "Engine/Core/Core.h"
#include "Engine/Manager/Device/Device.h"

#include "Game/Scenes/TestScene.h"

Game::Game() :
	m_GameState(GAME_STATE::PLAY)
{
	/* === === === === ===
	*       �⺻ ����
	* === === === === === */
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	// Backface culling ��� ����
	m_SceneMgr = new SceneMgr;
	
	/* === === === === ===
	*       Scene �߰�
	* === === === === === */
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