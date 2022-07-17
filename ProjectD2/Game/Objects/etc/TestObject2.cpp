#include "Framework.h"
#include "TestObject2.h"

TestObject2::TestObject2(Game* _game, Scene* _scene, GameObject* _parent) :
	TestObject2(_game, _scene, OBJECT_TYPE::DEFAULT ,_parent)
{
}

TestObject2::TestObject2(Game* _game, Scene* _scene, OBJECT_TYPE _type, GameObject* _parent) :
	GameObject(_game, _scene, _type, _parent)
	, m_isRight(true)
	, m_speed(100.f)
{
}

TestObject2::~TestObject2()
{
}

void TestObject2::UpdateObject()
{
}

void TestObject2::Render()
{
	GameObject::Render();
}
