#include "Framework.h"
#include "TestScene.h"

#include "Engine/Resource/Shader.h"
#include "Engine/Component/Component.h"
#include "Engine/Component/Collision/Collision.h"
#include "Game/Objects/etc/TestObject.h"
#include "Game/Objects/etc/TestObject2.h"
#include "Game/Objects/etc/TestObjectCircle.h"
#include "Game/Objects/etc/TestObjectLine.h"
#include "Game/Objects/etc/TestMap_1.h"

TestScene::TestScene(Game* _game) :
	Scene(_game)
{}

TestScene::~TestScene()
{}

void TestScene::Init()
{
	testObject = new TestObject(this, OBJECT_TYPE::PLAYER, 110);
	testObject->SetPos({ WIN_CENTER_X, WIN_CENTER_Y, 0.0f });
	
	testObject = new TestObject(this, OBJECT_TYPE::PLAYER, 110);
	testObject->SetPos({ WIN_CENTER_X + 100, WIN_CENTER_Y + 100, 0.0f });

	//testObject2 = new TestObject2(this, OBJECT_TYPE::FOREBLOCK);
	//testObject2->SetPos({ WIN_CENTER_X + 300, WIN_CENTER_Y, 0.0f });

	//testObjectCircle = new TestObjectCircle(this, OBJECT_TYPE::PLATFORM);
	//testObjectCircle->SetPos({ WIN_CENTER_X - 300, WIN_CENTER_Y, 0.0f });
	//testObjectCircle->SetScale(5.0f, 5.0f);

	//testline = new TestObjectLine(this, OBJECT_TYPE::PLATFORM);
	//testline->SetPos({ WIN_CENTER_X - 300, WIN_CENTER_Y, 0.0f });
	//testline->SetScale(-2, 2);

	testMap_1 = new TestMap_1(this);
	testMap_1->SetPos(WIN_CENTER_X, WIN_CENTER_Y);
	testMap_1->SetScale(1.5, 1.5);
}

void TestScene::Release()
{
}

void TestScene::UpdateScene()
{
	vector<std::pair<Collider*, Collider*>> collided;
	GetCollisionMgr()->CheckCollision(OBJECT_TYPE::PLAYER, OBJECT_TYPE::PLATFORM, collided);
	GetCollisionMgr()->CheckCollision(OBJECT_TYPE::PLAYER, OBJECT_TYPE::FOREBLOCK, collided);
}

void TestScene::RenderScene()
{
}