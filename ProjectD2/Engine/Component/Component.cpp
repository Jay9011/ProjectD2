#include "Framework.h"
#include "Component.h"

#include "Engine/Object/GameObject.h"
#include "Animator/Animator.h"
#include "Engine/Resource/Shader.h"
#include "Collision/Collider.h"
#include "Collision/Colliders/AARect.h"
#include "Collision/Colliders/Circle.h"
#include "Collision/Colliders/Line.h"
#include "PhysicsWorld/Physics.h"
#include "PhysicsWorld/Gravity/Gravity.h"

Component::Component(GameObject* _owner, int _updateOrder) :
	m_owner(_owner)
	, m_updateOrder(_updateOrder)
	, m_autoRendering(true)
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

Animator* Component::Factory::NewAnimator(GameObject* _owner, Shader* _shader, int _updateOrder)
{
	Animator* animator = new Animator(_owner, _updateOrder);
	animator->m_shader = _shader;
	
	return animator;
}

AARect* Component::Factory::NewAARect(const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, GameObject* _owner, int _updateOrder)
{
	return new AARect(_min, _max, _owner, _updateOrder);
}

AARect* Component::Factory::NewAARect(const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, OBJECT_TYPE _type, GameObject* _owner, int _updateOrder)
{
	return new AARect(_min, _max, _type, _owner, _updateOrder);
}

Circle* Component::Factory::NewCircle(const D3DXVECTOR2& _center, float _radius, GameObject* _owner, int _updateOrder)
{
	return new Circle(_center, _radius, _owner, _updateOrder);
}

Circle* Component::Factory::NewCircle(const D3DXVECTOR2& _center, float _radius, OBJECT_TYPE _type, GameObject* _owner, int _updateOrder)
{
	return new Circle(_center, _radius, _type, _owner, _updateOrder);
}

Line* Component::Factory::NewLine(const D3DXVECTOR2& _start, const D3DXVECTOR2& _end, GameObject* _owner, int _updateOrder)
{
	return new Line(_start, _end, _owner, _updateOrder);
}

Line* Component::Factory::NewLine(const D3DXVECTOR2& _start, const D3DXVECTOR2& _end, OBJECT_TYPE _type, GameObject* _owner, int _updateOrder)
{
	return new Line(_start, _end, _type, _owner, _updateOrder);
}

Gravity* Component::Factory::NewGravity(Physics& _physics, GameObject* _owner, int _updateOrder)
{
	return new Gravity(_physics, _owner, _updateOrder);
}

Gravity* Component::Factory::NewGravity(Physics& _physics, float _maxFallSpeed, GameObject* _owner, int _updateOrder)
{
	return new Gravity(_physics, _maxFallSpeed, _owner, _updateOrder);
}

Gravity* Component::Factory::NewGravity(Physics& _physics, float _maxFallSpeed, float _gravity, GameObject* _owner, int _updateOrder)
{
	return new Gravity(_physics, _maxFallSpeed, _gravity, _owner, _updateOrder);
}
