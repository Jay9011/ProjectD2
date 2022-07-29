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
{
}

TestScene::~TestScene() = default;

void TestScene::Init()
{
	testMap_1 = new TestMap_1(this);
	testMap_1->SetPos(WIN_CENTER_X, WIN_CENTER_Y);
	testMap_1->SetScale(1.5, 1.5);
}

void TestScene::SceneEnter()
{
	testObject = new TestObject(this, OBJECT_TYPE::PLAYER, 110);
	testObject->SetPos(testMap_1->GetPlayerStartPoint());
	testObject->SetScale(1.5, 1.5);
	testObjectPos = testObject->GetPos();

	CAMERA->SetTarget(testObject);
    CAMERA->SetOffset({WIN_CENTER_X, WIN_HEIGHT * 0.7f});
}

void TestScene::SceneExit()
{
}

void TestScene::Release()
{
}

void TestScene::UpdateScene()
{
	vector<std::pair<Collider*, Collider*>> collided;
	GetCollisionMgr()->CheckCollision(OBJECT_TYPE::PLAYER, OBJECT_TYPE::PLATFORM, collided);
}

void TestScene::RenderScene()
{
}
