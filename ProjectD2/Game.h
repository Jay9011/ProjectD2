#pragma once
#include "Engine/Manager/Scene/SceneMgr.h"
class Game
{
public:
	Game();
	~Game();

	void Update();
	void Render();
	
private:
	GAME_STATE m_GameState;
	SceneMgr*  m_SceneMgr;

public:
	GAME_STATE GetGameState() const { return m_GameState; }
	void       SetGameState(GAME_STATE _state) { m_GameState = _state; }
	
	SceneMgr*  GetSceneMgr() const { return m_SceneMgr; }
	Scene*     CurScene() const    { return m_SceneMgr->GetCurrentScene(); }
	Scene*     GetCurrrentScene() const { return m_SceneMgr->GetCurrentScene(); }
	
};
