#include "Framework.h"
#include "SceneMgr.h"

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

void SceneMgr::FinalUpdate()
{
	if (m_curScene == nullptr)
		return;

	m_curScene->FinalUpdate();

	ChangeNextScene();	// 현재 씬의 FinalUpdate까지 완료한 후 씬을 변경한다.
}

Scene* SceneMgr::Add(const string& _name, Scene* _scene)
{
	if (m_scenes.find(_name) != m_scenes.end())
		return m_scenes[_name];

	_scene->SetName(_name);
	m_scenes.emplace(_name, _scene);
	
	return _scene;
}
/*
* 사용자 호출용. (예약)
*/
void SceneMgr::ChangeScene(const string& _name)
{
	auto scene = m_scenes.find(_name);
	if(scene == m_scenes.end() || scene->second == m_curScene)
		return;

	m_nextScene = scene->second;	// 씬 예약
    
	if (m_curScene == nullptr)
	{
		ChangeNextScene();
	}
}
/*
* 실제 씬 변경용
*/
void SceneMgr::ChangeNextScene()
{
	if (m_nextScene == nullptr)
		return;
    
	if (m_curScene != nullptr)
	{
		m_curScene->Exit();
		m_curScene->Release();
	}

	m_curScene = m_nextScene;
	m_nextScene = nullptr;
    
	m_curScene->Init();
	m_curScene->Enter();
}
