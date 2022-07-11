#pragma once
class GameObject;
class Scene
{
public:
	Scene(Game* _game);
	virtual ~Scene();

	virtual void Init()    = 0;
	virtual void Release() = 0;
	
	virtual void Update();
	virtual void Render();

private:
	Game* m_game;
	string m_SceneName;
	
	vector<GameObject*> m_objects[(UINT)OBJECT_TYPE::FIN];
	vector<GameObject*> m_pendingObjects[(UINT)OBJECT_TYPE::FIN];	// 대기중인 오브젝트들

	bool m_updatingObjects;

	void AddObject(GameObject* _object, OBJECT_TYPE _type);
	void DeleteObject(GameObject* _object);

	void DeleteGroup(OBJECT_TYPE _type);
	void DeleteAll();
	
public:
	const string& GetName() const              { return m_SceneName; }
	void          SetName(const string& _name) { m_SceneName = _name; }

};
