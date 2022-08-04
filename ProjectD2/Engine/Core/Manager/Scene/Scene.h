#pragma once
class Game;
class GameObject;
class UIObject;
class Scene
{
public:
	Scene(Game* _game);
	virtual ~Scene();

	virtual void Init()    = 0;
	virtual void Release() = 0;
	virtual void UpdateScene() = 0;
	virtual void RenderScene() = 0;

	virtual void SceneEnter() = 0;
	virtual void SceneExit()  = 0;
	
	void Enter();
	void Exit();

	void Update();
	void FinalUpdate();
	void Render();

	void AddObject(GameObject* _object, OBJECT_TYPE _type);
	void DeleteObject(GameObject* _object);
	
	void AddUI(UIObject* _ui);
	void DeleteUI(UIObject* _ui);

	void DeleteGroup(OBJECT_TYPE _type);
	void DeleteAll();

private:
	Game* m_game;
	string m_SceneName;
	
	class CollisionMgr* m_CollisionMgr;
	
	bool m_updatingObjects;
	
	vector<GameObject*> m_objects[(UINT)OBJECT_TYPE::FIN];
	vector<GameObject*> m_pendingObjects[(UINT)OBJECT_TYPE::FIN];	// 대기중인 오브젝트들
	list<GameObject*> m_deadObjects;	// 삭제 예정 죽은 오브젝트들
    
	vector<UIObject*> m_uiObjects;
	
/* === === === === ===
*  Getter / Setter
*  === === === === === */
public:
	const string& GetName() const              { return m_SceneName; }
	void          SetName(const string& _name) { m_SceneName = _name; }
	Game*         GetGame() const              { return m_game; }
	CollisionMgr* GetCollisionMgr() const      { return m_CollisionMgr; }

#if _DEBUG
public:
	TwBar* twbar = nullptr;
#endif // _DEBUG
};
