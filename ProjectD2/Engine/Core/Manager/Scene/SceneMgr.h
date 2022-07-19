#pragma once
#include "Scene.h"
class SceneMgr
{
public:
	SceneMgr();
	~SceneMgr();

	void Update();
	void Render();
	void FinalUpdate();
	
	Scene* Add(const string& _name, Scene* _scene);
	void   ChangeScene(const string& _name);
	
	Scene* GetCurrentScene() const { return m_curScene; }

private:
	unordered_map<string, Scene*> m_scenes;
	Scene* m_curScene;
	
};
