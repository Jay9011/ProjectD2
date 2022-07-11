#include "Framework.h"
#include "GameObject.h"

#include "Game.h"
#include "Engine/Component/Component.h"

GameObject::GameObject(Game* _game) :
	m_game(_game)
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
}

void GameObject::Update()
{
}

void GameObject::UpdateWorld()
{
}

void GameObject::Render()
{
}

void GameObject::UpdateObject()
{
}

void GameObject::UpdateComponent()
{
}

void GameObject::AddComponent(Component* _component)
{
}

void GameObject::RemoveComponent(Component* _component)
{
}
