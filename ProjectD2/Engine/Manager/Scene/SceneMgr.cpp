#include "Framework.h"
#include "SceneMgr.h"
#include "Scene.h"

SceneMgr::SceneMgr()
{
}

SceneMgr::~SceneMgr()
{
	m_curScene->Release();

	SAFE_DELETE_MAP(m_scenes);
}

void SceneMgr::Update()
{
	if (m_curScene == nullptr)
		return;

	m_curScene->Update();
}

void SceneMgr::Render()
{
	if (m_curScene == nullptr)
		return;

	m_curScene->Render();
}

Scene* SceneMgr::Add(const string& _name, Scene* _scene)
{
	if (m_scenes.find(_name) != m_scenes.end())
		return m_scenes[_name];

	m_scenes.emplace(_name, _scene);
	
	return _scene;
}

void SceneMgr::ChangeScene(const string& _name)
{
	auto scene = m_scenes.find(_name);
	if(scene == m_scenes.end() || scene->second == m_curScene)
		return;
	
	if (m_curScene != nullptr)
		m_curScene->Release();

	m_curScene = m_scenes[_name];
	m_curScene->Init();
}
