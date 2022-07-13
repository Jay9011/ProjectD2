#include "Framework.h"
#include "Scene.h"

#include "Engine/Object/GameObject.h"

Scene::Scene(Game* _game) :
	m_game(_game)
	, m_updatingObjects(false)
{
#if _DEBUG
	twbar = TweakBar::Get()->GetBar();
#endif // _DEBUG

}

Scene::~Scene()
{
	DeleteAll();
}

void Scene::Update()
{
	if (m_game->GetGameState() == GAME_STATE::PLAY)
	{
		std::vector<GameObject*> deadObjects; // 죽은 object들 저장용 벡터

		/* === === === === ===
		*   Objects Update
		* === === === === === */
		m_updatingObjects = true;
		for (auto& objectTypeVec : m_objects)
		{
			for (auto& object : objectTypeVec)
			{
				if (object->IsDead())
				{
					deadObjects.emplace_back(object);
					continue;
				}

				object->Update();
			}
		}
		m_updatingObjects = false;

		// 대기중인 object들 갱신후 이동
		for (UINT i = 0; i < (UINT)OBJECT_TYPE::FIN; i++)
		{
			for (auto& object : m_pendingObjects[i])
			{
				object->UpdateWorld();
				m_objects[i].emplace_back(object);
			}
			m_pendingObjects[i].clear();
		}

		SAFE_DELETE_VEC(deadObjects);
	}

	UpdateScene();
}

void Scene::Render()
{
	for (auto& objectTypeVector : m_objects)
	{
		for (auto& object : objectTypeVector)
		{
			object->Render();
		}
	}

	RenderScene();
}

void Scene::AddObject(GameObject* _object, OBJECT_TYPE _type)
{
	_object->SetType(_type);

	if (m_updatingObjects)
	{
		m_pendingObjects[(UINT)_type].emplace_back(_object);
	}
	else
	{
		m_objects[(UINT)_type].emplace_back(_object);
	}
}

void Scene::DeleteObject(GameObject* _object)
{
	OBJECT_TYPE type = _object->GetType();
	auto iter = std::find(m_pendingObjects[(UINT)type].begin(), m_pendingObjects[(UINT)type].end(), _object);
	
	if(iter != m_pendingObjects[(UINT)type].end())
	{
		std::iter_swap(iter, m_pendingObjects[(UINT)type].end() - 1);
		m_pendingObjects[(UINT)type].pop_back();
	}
	else
	{
		iter = std::find(m_objects[(UINT)type].begin(), m_objects[(UINT)type].end(), _object);
		if (iter != m_objects[(UINT)type].end())
		{
			std::iter_swap(iter, m_objects[(UINT)type].end() - 1);
			m_objects[(UINT)type].pop_back();
		}
	}
}

void Scene::DeleteGroup(OBJECT_TYPE _type)
{
	for (auto& object : m_objects[(UINT)_type])
	{
		SAFE_DELETE(object);
	}
	m_objects[(UINT)_type].clear();
	
	for (auto& object : m_pendingObjects[(UINT)_type])
	{
		SAFE_DELETE(object);
	}
	m_pendingObjects[(UINT)_type].clear();
}

void Scene::DeleteAll()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::FIN; i++)
	{
		DeleteGroup((OBJECT_TYPE)i);
	}
}
