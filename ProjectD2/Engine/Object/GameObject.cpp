#include "Framework.h"
#include "GameObject.h"

#include "Engine/Core/Core.h"
#include "Engine/Component/Component.h"

GameObject::GameObject(Game* _game, Scene* _scene, GameObject* _parent) :
	GameObject(_game, _scene, OBJECT_TYPE::DEFAULT, _parent)
{}

GameObject::GameObject(Game* _game, Scene* _scene, OBJECT_TYPE _type, GameObject* _parent) :
	m_game(_game)
	, m_scene(_scene)
	, m_parent(_parent)
	, m_type(_type)
	, m_state(OBJECT_STATE::ACTIVE)
	, m_pos(0, 0, 0)
	, m_scale(1, 1, 1)
	, m_angle(0)
	, m_pivot(0, 0, 0)
	, m_recomputeWorld(false)
{
	D3DXMatrixIdentity(&m_S);
	D3DXMatrixIdentity(&m_R);
	D3DXMatrixIdentity(&m_T);
	D3DXMatrixIdentity(&m_world);

	m_scene->AddObject(this, _type);

#if _DEBUG
	SetVertexData();
#endif // _DEBUG

}

GameObject::~GameObject()
{
	m_scene->DeleteObject(this);

	SAFE_DELETE_VEC(m_componentList);
}

void GameObject::Update()
{
	if (m_state == OBJECT_STATE::ACTIVE)
	{
		UpdateWorld();

		UpdateComponent();
		UpdateObject();
		
		UpdateWorld();

		SetWorld();
	}
}

void GameObject::UpdateWorld()
{
	if (m_recomputeWorld)
	{
		D3DXMatrixScaling(&m_S, m_scale.x, m_scale.y, m_scale.z);
		D3DXMatrixRotationZ(&m_R, m_angle);
		D3DXMatrixTranslation(&m_T, m_pos.x, m_pos.y, m_pos.z);

		D3DXMATRIX P;
		D3DXMATRIX IP;
		D3DXMatrixTranslation(&P, m_pivot.x, m_pivot.y, m_pivot.z);
		D3DXMatrixInverse(&IP, nullptr, &P);

		m_world = IP * m_R * m_S * m_T * P;
		
		m_recomputeWorld = false;
	}
}

void GameObject::SetWorld()
{
	DEVICE->SetTransform(D3DTS_WORLD, &m_world);
}

void GameObject::UpdateObject()
{
}

void GameObject::UpdateComponent()
{
	for (auto& component : m_componentList)
	{
		component->Update();
	}
}

void GameObject::Render()
{
	for (auto& component : m_componentList)
	{
		component->Render();
	}
#if _DEBUG
	DEVICE->SetFVF(VERTEXCOLOR::FVF);
	DEVICE->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, m_vertexList.size(), m_indexList.size() / 2, m_indexList.data(), D3DFMT_INDEX16, m_vertexList.data(), sizeof(VERTEXCOLOR));
#endif // 테스트 렌더링 (Object 위치 표시)
}

void GameObject::AddComponent(Component* _component)
{
	int componentOrder = _component->GetUpdateOrder();
	auto iter = m_componentList.begin();
	for (; iter != m_componentList.end(); iter++)
	{
		if (componentOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	
	m_componentList.insert(iter, _component);
}

void GameObject::RemoveComponent(Component* _component)
{
	auto iter = std::find(m_componentList.begin(), m_componentList.end(), _component);
	if (iter != m_componentList.end())
	{
		m_componentList.erase(iter);
	}
}

void GameObject::SetVertexData()
{
	float angle = PI * 2.f * 0.083f;

	D3DCOLOR color = 0xFF3AB0FF;
	
	for (int i = 0; i < 13; i++)
	{
		D3DXVECTOR3 pos;
		pos.x =  cos(angle * (float)i) * 8.f;
		pos.y = -sin(angle * (float)i) * 8.f;
		
		m_vertexList.push_back(VERTEXCOLOR(pos.x, pos.y, color));
		m_indexList.push_back((WORD)i);
		m_indexList.push_back((WORD)i + 1);
	}
	m_indexList.pop_back();
	m_indexList.push_back(0);
	
	m_indexList.push_back((WORD)m_vertexList.size());
	m_indexList.push_back((WORD)m_vertexList.size() + 1);
	m_vertexList.push_back(VERTEXCOLOR(0, -12, color));
	m_vertexList.push_back(VERTEXCOLOR(0, +12, color));
	
	m_indexList.push_back((WORD)m_vertexList.size());
	m_indexList.push_back((WORD)m_vertexList.size() + 1);
	m_vertexList.push_back(VERTEXCOLOR(-12, 0, color));
	m_vertexList.push_back(VERTEXCOLOR(+12, 0, color));
}
