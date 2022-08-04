#pragma once
#include "Engine/Observer/Observer.h"
enum class GAME_STATE
{
	PLAY,
	PAUSE,
	EXIT
};

class Game
{
public:
	Game();
	~Game();

public:
	void Update();
	void Render();
	void FinalUpdate();

private:
	GAME_STATE m_GameState;
	static bool m_isDbgRendering;

public:
	Observable<class Player>* playerObservable;

/* === === === === ===
* Getter / Setter
* === === === === ===*/
public:
	GAME_STATE GetGameState() const { return m_GameState; }
	void       SetGameState(GAME_STATE _state) { m_GameState = _state; }

	Scene* CurScene() const { return SCENE->GetCurrentScene(); }
	Scene* GetCurrrentScene() const { return SCENE->GetCurrentScene(); }

	static bool	IsDbgRendering() { return m_isDbgRendering; }

private:
	bool& ChangeDbgRendering() { return m_isDbgRendering = !m_isDbgRendering; }
	void ChangeGameState() { m_GameState = (m_GameState == GAME_STATE::PLAY) ? GAME_STATE::PAUSE : GAME_STATE::PLAY; }

/* === === === === ===
*    TWBar ฐüทร
* === === === === ===*/
#if _DEBUG
private:
	string m_CurSceneName;

#endif // _DEBUG
};
