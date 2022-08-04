#include "Framework.h"
#include "Scene.h"

#include "Engine/Object/GameObject.h"
#include "Engine/Object/UIObject.h"
#include "Engine/Component/Collision/Collision.h"

Scene::Scene(Game* _game) :
	m_game(_game)
	, m_updatingObjects(false)
{
#if _DEBUG
	twbar = TweakBar::Get()->GetBar();
#endif // _DEBUG

	m_CollisionMgr = new CollisionMgr(_game);

}

Scene::~Scene()
{
	DeleteAll();
	delete m_CollisionMgr;
}

void Scene::Enter()
{
	// Scene의 Object들이 다 만들어지면 Object들의 World 좌표로 변환 시킨다.
	m_updatingObjects = true;
	for (auto& objectTypeVec : m_objects)
	{
		for (auto& object : objectTypeVec)
		{
			object->UpdateWorld();
		}
	}
	// 부모 좌표를 적용받지 못한 (먼저 UpdateWorld 된) Object들을 적용받게 한다.
	for (auto& objectTypeVec : m_objects)
	{
		for (auto& object : objectTypeVec)
		{
			object->IsRecomputeWorld(true);
			object->UpdateWorld();
		}
	}
	m_updatingObjects = false;

	SceneEnter();
}

void Scene::Exit()
{
	
	SceneExit();
}

void Scene::Update()
{
	if (m_game->GetGameState() == GAME_STATE::PLAY)
	{
		list<GameObject*> deadObjects;	// 이번 프레임에 죽은 object들 저장용 벡터

		/* === === === === ===
		*   Objects Update
		* === === === === === */
		m_updatingObjects = true;
		for (auto& objectTypeVec : m_objects)
		{
            auto iter = objectTypeVec.begin();
            while (iter != objectTypeVec.end())
            {
                if ((*iter)->IsDead())
                {
                    deadObjects.emplace_back(*iter);
                    iter = objectTypeVec.erase(iter);
					continue;
                }
                
				if ((*iter)->GetState() == OBJECT_STATE::INACTIVE)
				{
                    ++iter;
                    continue;
				}
                
                (*iter)->Update();
                ++iter;
            }
		}
        /* === === === === ===
		*  UI Objects Update
		* === === === === ===*/
        auto iter = m_uiObjects.begin();
        while (iter != m_uiObjects.end())
        {
            if ((*iter)->IsInactive())
                continue;
            
            (*iter)->Update();
            ++iter;
        }
        
		m_updatingObjects = false;

		// 대기중인 object들 갱신후 이동
		for (UINT i = 0; i < (UINT)OBJECT_TYPE::FIN; i++)
		{
			for (auto& object : m_pendingObjects[i])
			{
				object->UpdateWorld();
				AddObject(object, (OBJECT_TYPE)i);
			}
			m_pendingObjects[i].clear();
		}

        // 이전 프레임에 죽은 오브젝트들 삭제
        for (auto& object : m_deadObjects)
        {
            SAFE_DELETE(object);
        }
        m_deadObjects.clear();
        
        std::swap(m_deadObjects, deadObjects);
	}

	UpdateScene();

	m_CollisionMgr->Update();
}

void Scene::FinalUpdate()
{
	if (m_game->GetGameState() == GAME_STATE::PLAY)
	{
		m_updatingObjects = true;
		
		for (auto& objectTypeVec : m_objects)
		{
			for (auto& object : objectTypeVec)
			{
				if (object->GetState() == OBJECT_STATE::INACTIVE)	// Inactive 상태인 것은 넘긴다.
					continue;
                
				object->FinalUpdate();
			}
		}

		/* === === === === ===
		*  UI Objects Final Update
		* === === === === ===*/
        for (auto& object : m_uiObjects)
        {
            if (object->IsInactive())
                continue;
            
            object->FinalUpdate();
        }

		m_updatingObjects = false;
	}
	
	m_CollisionMgr->FinalUpdate();
}

void Scene::Render()
{
	for (auto& objectTypeVector : m_objects)
	{
		for (auto& object : objectTypeVector)
		{
			if (object->GetState() == OBJECT_STATE::INACTIVE)	// Inactive 상태인 것은 넘긴다.
				continue;
            
			object->Render();
		}
	}

	RenderScene();

    /* === === === === ===
	* 마지막에 UI를 Render한다.
	* === === === === === */
    for (auto& ui : m_uiObjects)
    {
        if (ui->IsHidden() || ui->IsInactive())
            continue;
        
        ui->Render();
    }
	CAMERA->SetView(); // UI Render를 마친 후 View를 원래 상태로 되돌린다.
    
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
		int objectOrder = _object->GetOrder();
		auto iter = m_objects[(UINT)_type].begin();
		for (; iter != m_objects[(UINT)_type].end(); iter++)
		{
			if (objectOrder < (*iter)->GetOrder())
			{
				break;
			}
		}
		
		m_objects[(UINT)_type].insert(iter, _object);
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

void Scene::AddUI(UIObject* _ui)
{
	int objectOrder = _ui->GetOrder();
	auto iter = m_uiObjects.begin();
	for (; iter != m_uiObjects.end(); iter++)
	{
		if (objectOrder < (*iter)->GetOrder())
		{
			break;
		}
	}

	m_uiObjects.insert(iter, _ui);
}

void Scene::DeleteUI(UIObject* _ui)
{
	auto iter = std::find(m_uiObjects.begin(), m_uiObjects.end(), _ui);
	if(iter != m_uiObjects.end())
    {
        std::iter_swap(iter, m_uiObjects.end() - 1);
        m_uiObjects.pop_back();
    }
}

void Scene::DeleteGroup(OBJECT_TYPE _type)
{
	while (!m_objects[(UINT)_type].empty())
	{
		delete m_objects[(UINT)_type].back();
	}
	m_objects[(UINT)_type].clear();

	while (!m_pendingObjects[(UINT)_type].empty())
	{
		delete m_pendingObjects[(UINT)_type].back();
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
