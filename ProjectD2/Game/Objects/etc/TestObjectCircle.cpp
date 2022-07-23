#include "Framework.h"
#include "TestObjectCircle.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/Collision/Collision.h"

TestObjectCircle::TestObjectCircle(Scene* _scene, int _updateOrder, GameObject* _parent) :
	TestObjectCircle(_scene, OBJECT_TYPE::DEFAULT, _updateOrder, _parent)
{
}

TestObjectCircle::TestObjectCircle(Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, _type, _updateOrder, _parent)
	, m_isRight(true)
	, m_speed(100.f)
{
	m_bodyCollider = ADDCOMP::NewCircle({ 0, 0 }, 30.f, this);
	m_bodyCollider->IsActive(true);
}

TestObjectCircle::~TestObjectCircle()
{
}

void TestObjectCircle::UpdateObject()
{
}

void TestObjectCircle::RenderObject()
{
	GameObject::RenderObject();
}
