#include "Framework.h"
#include "TestObject2.h"

TestObject2::TestObject2(Scene* _scene, GameObject* _parent) :
	TestObject2(_scene, OBJECT_TYPE::DEFAULT ,_parent)
{
}

TestObject2::TestObject2(Scene* _scene, OBJECT_TYPE _type, GameObject* _parent) :
	GameObject(_scene, _type, _parent)
	, m_isRight(true)
	, m_speed(100.f)
{
}

TestObject2::~TestObject2()
{
}

void TestObject2::FinalUpdate()
{
}

void TestObject2::FinalRender()
{
	GameObject::FinalRender();
}
