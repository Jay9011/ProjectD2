#pragma once
class Game
{
public:
	Game();
	~Game();

	void Update();
	void Render();
	void FinalUpdate();
	
private:
	GAME_STATE m_GameState;

public:
	GAME_STATE GetGameState() const { return m_GameState; }
	void       SetGameState(GAME_STATE _state) { m_GameState = _state; }
	
	Scene*     CurScene() const    { return SCENE->GetCurrentScene(); }
	Scene*     GetCurrrentScene() const { return SCENE->GetCurrentScene(); }
	
/* === === === === ===
*    TWBar ฐüทร
* === === === === ===*/
#if _DEBUG
private:
	string m_CurSceneName;

#endif // _DEBUG
};
