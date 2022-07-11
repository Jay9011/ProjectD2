#include "Framework.h"
#include "GameObject.h"

#include "Engine/Core/Core.h"
#include "Engine/Component/Component.h"

GameObject::GameObject(Game* _game) :
	m_game(_game)
	, m_type(OBJECT_TYPE::DEFAULT)
	, m_state(OBJECT_STATE::ACTIVE)
	, m_parent(nullptr)
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

}

GameObject::~GameObject()
{
	m_game->CurScene()->DeleteObject(this);

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
