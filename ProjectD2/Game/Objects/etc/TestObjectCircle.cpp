#include "Framework.h"
#include "TestObjectCircle.h"

#include "Engine/Component/Collision/Colliders/Circle.h"

TestObjectCircle::TestObjectCircle(Scene* _scene, GameObject* _parent) :
	TestObjectCircle(_scene, OBJECT_TYPE::DEFAULT ,_parent)
{
}

TestObjectCircle::TestObjectCircle(Scene* _scene, OBJECT_TYPE _type, GameObject* _parent) :
	GameObject(_scene, _type, _parent)
	, m_isRight(true)
	, m_speed(100.f)
{
	m_bodyCollider = new Circle({ WIN_CENTER_X, WIN_CENTER_Y - 300}, 100.f, this);
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
