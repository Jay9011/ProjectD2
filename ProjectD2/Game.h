#pragma once
class Scene;
class Game
{
public:
	Game();
	~Game();

	void Run();
	
	void Update();
	void Render();

	
private:
	GAME_STATE m_GameState;
	Scene* m_scene;

public:
	GAME_STATE GetGameState() const { return m_GameState; }
	void       SetGameState(GAME_STATE _state) { m_GameState = _state; }
	
};
