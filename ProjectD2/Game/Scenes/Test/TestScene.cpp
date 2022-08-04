#include "Framework.h"
#include "TestScene.h"

#include "Engine/Resource/Shader.h"
#include "Engine/Component/Component.h"
#include "Engine/Component/Collision/Collision.h"
#include "Game/Objects/Charactor/Player.h"
#include "Game/Objects/etc/TestObject2.h"
#include "Game/Objects/etc/TestObjectCircle.h"
#include "Game/Objects/etc/TestObjectLine.h"
#include "Game/Objects/etc/TestMap_1.h"

#include "Game/Objects/UI/WeaponUI.h"
#include "Game/Objects/UI/HeartUI.h"

TestScene::TestScene(Game* _game) :
	Scene(_game)
{
}

TestScene::~TestScene() = default;

void TestScene::Init()
{
	testMap_1 = new TestMap_1(this);
	testMap_1->SetScale(1.5, 1.5);
	testMap_1->SetPos(WIN_CENTER_X, WIN_CENTER_Y);

    /*
	* UI 생성
	*/
	playerWeapon = new WeaponUI(this);
    playerWeapon->SetPos({ 80.0f, 50.0f });
	playerWeapon->SetScale(0.5f, 0.5f);
    
	playerHeart = new HeartUI(this);
    playerHeart->SetPos({150.0f, 30.0f});
    playerHeart->SetScale(0.2f, 0.2f);
}

void TestScene::SceneEnter()
{
	testObject = new Player(this, OBJECT_TYPE::PLAYER, 110);
	testObject->SetScale(1.5, 1.5);
	testObject->SetPos(testMap_1->GetPlayerStartPoint());
	
	CAMERA->SetLookAt(testObject->GetPos());
	CAMERA->SetTarget(testObject);
	testMap_1->CameraInit();
}

void TestScene::SceneExit()
{
}

void TestScene::Release()
{
}

void TestScene::UpdateScene()
{
    /*
	* 카메라의 타겟 확인
	*/
	if (testObject->IsDead())
	{
        CAMERA->SetTarget(nullptr);
    }
    /*
	* 글로벌 충돌 확인
	*/
	vector<std::pair<Collider*, Collider*>> collided;
	GetCollisionMgr()->CheckCollision(OBJECT_TYPE::PLAYER, OBJECT_TYPE::PLATFORM, collided);
	GetCollisionMgr()->CheckCollision(OBJECT_TYPE::PLAYER_ATK, OBJECT_TYPE::MONSTER, collided);
	GetCollisionMgr()->CheckCollision(OBJECT_TYPE::MONSTER_ATK, OBJECT_TYPE::PLAYER, collided);

}

void TestScene::RenderScene()
{
}
