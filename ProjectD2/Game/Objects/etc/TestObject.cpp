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
#if _DEBUG
	TwAddVarRW(_scene->twbar, "Speed", TW_TYPE_FLOAT, &m_speed, "min=0.0 max=1000.0 step=1.0");
#endif // _DEBUG

}

TestObject::~TestObject()
{
}

void TestObject::UpdateObject()
{
	if(KEYPRESS(VK_LEFT))
	{
		AddPos(V_LEFT * m_speed * DT);
	}
	if (KEYPRESS(VK_RIGHT))
	{
		AddPos(V_RIGHT * m_speed * DT);
	}
	if (KEYPRESS(VK_UP))
	{
		AddPos(V_UP * m_speed * DT);
	}
	if (KEYPRESS(VK_DOWN))
	{
		AddPos(V_DOWN * m_speed * DT);
	}
}
