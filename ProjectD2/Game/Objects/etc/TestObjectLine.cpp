#include "Framework.h"
#include "TestObjectLine.h"

#include "Engine/Object/GameObject.h"
#include "Engine/Component/Component.h"
#include "Engine/Component/Collision/Colliders/Line.h"

TestObjectLine::TestObjectLine(Scene* _scene, int _updateOrder, GameObject* _parent) :
	TestObjectLine(_scene, OBJECT_TYPE::DEFAULT, _updateOrder, _parent)
{
}

TestObjectLine::TestObjectLine(Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, _type, _updateOrder, _parent)
{
	m_line = ADDCOMP::NewLine({ -20, 20 }, { 20, -20 }, this);
	m_line->IsActive(true);
}

TestObjectLine::~TestObjectLine()
{
}

void TestObjectLine::UpdateObject()
{
}

void TestObjectLine::RenderObject()
{
	GameObject::RenderObject();
}
