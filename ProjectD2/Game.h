#pragma once
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
	GAME_STATE GetGameState() const { return m_GameState; }
	void       SetGameState(GAME_STATE _state) { m_GameState = _state; }
	
	Scene*     CurScene() const    { return SCENE->GetCurrentScene(); }
	Scene*     GetCurrrentScene() const { return SCENE->GetCurrentScene(); }

	static bool	IsDbgRendering() { return m_isDbgRendering; }

private:
	bool& ChangeDbgRendering() { return m_isDbgRendering = !m_isDbgRendering; }
	void ChangeGameState(){ m_GameState = (m_GameState == GAME_STATE::PLAY) ? GAME_STATE::PAUSE : GAME_STATE::PLAY; }

/* === === === === ===
*    TWBar ฐüทร
* === === === === ===*/
#if _DEBUG
private:
	string m_CurSceneName;

#endif // _DEBUG
};
