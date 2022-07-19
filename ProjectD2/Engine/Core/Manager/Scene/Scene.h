#pragma once
class Game;
class GameObject;
class Scene
{
public:
	Scene(Game* _game);
	virtual ~Scene();

	virtual void Init()    = 0;
	virtual void Release() = 0;
	virtual void UpdateScene() = 0;
	virtual void RenderScene() = 0;
	
	void Update();
	void Render();

	void AddObject(GameObject* _object, OBJECT_TYPE _type);
	void DeleteObject(GameObject* _object);

	void DeleteGroup(OBJECT_TYPE _type);
	void DeleteAll();

private:
	Game* m_game;
	string m_SceneName;
	
	vector<GameObject*> m_objects[(UINT)OBJECT_TYPE::FIN];
	vector<GameObject*> m_pendingObjects[(UINT)OBJECT_TYPE::FIN];	// 대기중인 오브젝트들

	bool m_updatingObjects;
	
public:
	const string& GetName() const              { return m_SceneName; }
	void          SetName(const string& _name) { m_SceneName = _name; }
	Game*         GetGame() const              { return m_game; }

#if _DEBUG
public:
	TwBar* twbar = nullptr;
#endif // _DEBUG
};
