#include "Framework.h"
#include "TestObject.h"

TestObject::TestObject(Game* _game, Scene* _scene, GameObject* _parent) :
	TestObject(_game, _scene, OBJECT_TYPE::DEFAULT, _parent)
{
}

TestObject::TestObject(Game* _game, Scene* _scene, OBJECT_TYPE _type, GameObject* _parent) :
	GameObject(_game, _scene, _type, _parent)
	, m_speed(100.f)
{
}

TestObject::~TestObject()
{
}

void TestObject::UpdateObject()
{
	if(KEYPRESS(VK_LEFT))
	{
		AddPos(V_LEFT * 100.f * DT);
	}
	if (KEYPRESS(VK_RIGHT))
	{
		AddPos(V_RIGHT * 100.f * DT);
	}
	if (KEYPRESS(VK_UP))
	{
		AddPos(V_UP * 100.f * DT);
	}
	if (KEYPRESS(VK_DOWN))
	{
		AddPos(V_DOWN * 100.f * DT);
	}
}
