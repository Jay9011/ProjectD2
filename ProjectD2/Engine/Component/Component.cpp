#include "Framework.h"
#include "Component.h"

#include "Engine/Object/GameObject.h"

Component::Component(GameObject* _owner, int updateOrder) :
	m_owner(_owner)
	, m_updateOrder(updateOrder)
{
	m_owner->AddComponent(this);
}

Component::~Component()
{
	m_owner->RemoveComponent(this);
}
