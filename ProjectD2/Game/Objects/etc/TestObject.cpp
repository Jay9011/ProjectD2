#include "Framework.h"
#include "TestObject.h"

TestObject::TestObject(Game* _game, Scene* _scene, GameObject* _parent) :
	GameObject(_game, _scene, _parent)
{
}

TestObject::TestObject(Game* _game, Scene* _scene, OBJECT_TYPE _type, GameObject* _parent) :
	GameObject(_game, _scene, _type, _parent)
{
}

TestObject::~TestObject()
{
}

void TestObject::UpdateObject()
{
}
