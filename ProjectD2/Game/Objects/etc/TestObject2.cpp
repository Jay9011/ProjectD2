#include "Framework.h"
#include "TestObject2.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/Collision/Collision.h"

TestObject2::TestObject2(Scene* _scene, GameObject* _parent) :
	TestObject2(_scene, OBJECT_TYPE::DEFAULT ,_parent)
{
}

TestObject2::TestObject2(Scene* _scene, OBJECT_TYPE _type, GameObject* _parent) :
	GameObject(_scene, _type, _parent)
	, m_isRight(true)
	, m_speed(100.f)
{
	m_bodyCollider = ADDCOMP::NewAARect({ -100, -100 }, { 100, 100 }, this);
	m_bodyCollider->IsActive(true);
}

TestObject2::~TestObject2()
{
}

void TestObject2::UpdateObject()
{
}

void TestObject2::RenderObject()
{
	GameObject::RenderObject();
}
