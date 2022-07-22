#include "Framework.h"
#include "Component.h"

#include "Engine/Object/GameObject.h"
#include "Animator/Animator.h"
#include "Collision/Collider.h"
#include "Collision/Colliders/AARect.h"
#include "Collision/Colliders/Circle.h"
#include "Collision/Colliders/Line.h"

Component::Component(GameObject* _owner, int _updateOrder) :
	m_owner(_owner)
	, m_updateOrder(_updateOrder)
{
	m_owner->AddComponent(this);
}

Component::~Component()
{
	m_owner->RemoveComponent(this);
}

Animator* Component::Factory::NewAnimator(GameObject* _owner, int _updateOrder)
{
	return new Animator(_owner, _updateOrder);
}

AARect* Component::Factory::NewAARect(const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, GameObject* _owner, int _updateOrder)
{
	return new AARect(_min, _max, _owner, _updateOrder);
}

Circle* Component::Factory::NewCircle(const D3DXVECTOR2& _center, float _radius, GameObject* _owner, int _updateOrder)
{
	return new Circle(_center, _radius, _owner, _updateOrder);
}

Line* Component::Factory::NewLine(const D3DXVECTOR2& _start, const D3DXVECTOR2& _end, GameObject* _owner, int _updateOrder)
{
	return new Line(_start, _end, _owner, _updateOrder);
}
