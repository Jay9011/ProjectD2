#include "Framework.h"
#include "TestScene.h"

#include "Engine/Component/Collision/Collision.h"
#include "Game/Objects/etc/TestObject.h"
#include "Game/Objects/etc/TestObject2.h"
#include "Game/Objects/etc/TestObjectCircle.h"

TestScene::TestScene(Game* _game) :
	Scene(_game)
{}

TestScene::~TestScene()
{}

void TestScene::Init()
{
	testObject = new TestObject(this);
	testObject->SetPos({ WIN_CENTER_X, WIN_CENTER_Y, 0.5f });

	testObject2 = new TestObject2(this);
	testObject2->SetPos({ WIN_CENTER_X + 300, WIN_CENTER_Y, 0.0f });

	testObjectCircle = new TestObjectCircle(this);
	testObjectCircle->SetPos({ WIN_CENTER_X - 300, WIN_CENTER_Y, 0.0f });
	testObjectCircle->SetScale(5.0f, 5.0f);
}

void TestScene::Release()
{
}

void TestScene::UpdateScene()
{
	Collision(testObject->m_bodyCollider, testObject2->m_bodyCollider);
	Collision(testObject->m_bodyCollider, testObjectCircle->m_bodyCollider);
	Collision(testObject2->m_bodyCollider, testObject->m_sight_u);

	Collision(testObjectCircle->m_bodyCollider, MOUSEPOS);
}

void TestScene::RenderScene()
{
}