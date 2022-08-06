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
#include "Game/Objects/UI/DialogUI.h"
#include "Game/Objects/UI/DialogEvent.h"

TestScene::TestScene(Game* _game) :
	Scene(_game)
{
}

TestScene::~TestScene() = default;

void TestScene::Init()
{
    /*
	* UI 생성
	*/
	playerWeaponUI = new WeaponUI(this);
    playerWeaponUI->SetPos({ 80.0f, 50.0f });
	playerWeaponUI->SetScale(0.5f, 0.5f);
    
	playerHeartUI = new HeartUI(this);
    playerHeartUI->SetPos({150.0f, 30.0f});
    playerHeartUI->SetScale(0.2f, 0.2f);

    dialogUI = new DialogUI(this);
    dialogUI->SetPos({ WIN_CENTER_X, WIN_HEIGHT - 100.0f });

    /*
	* Map 생성
	*/
	testMap_1 = new TestMap_1(this);
	testMap_1->SetScale(1.5, 1.5);
	testMap_1->SetPos(WIN_CENTER_X, WIN_CENTER_Y);

}

void TestScene::SceneEnter()
{
	PlayerSpawn();
	CAMERA->SetLookAt(player->GetPos());
    
	testMap_1->CameraInit();
	testMap_1->SetPlayer(player);
	testMap_1->SetDialogUI(dialogUI);

	SOUND->Play("BGM", 0.15f, true);
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
	* 플레이어의 죽음
	*/
	if (player->IsDead())
	{
        CAMERA->SetTarget(nullptr);
		PlayerDieEvent();
    }
    /*
	* 글로벌 충돌 확인
	*/
	vector<std::pair<Collider*, Collider*>> collided;
	GetCollisionMgr()->CheckCollision(OBJECT_TYPE::PLAYER, OBJECT_TYPE::PLATFORM, collided);
	GetCollisionMgr()->CheckCollision(OBJECT_TYPE::PLAYER_ATK, OBJECT_TYPE::MONSTER, collided);
	GetCollisionMgr()->CheckCollision(OBJECT_TYPE::MONSTER_ATK, OBJECT_TYPE::PLAYER, collided);
	
    /*
	* Interactive Object 사용 확인
	*/
	GetCollisionMgr()->CheckCollision(OBJECT_TYPE::PLAYER, OBJECT_TYPE::INTERACTIVE, collided);
	GetCollisionMgr()->CheckCollision(OBJECT_TYPE::PLAYER, OBJECT_TYPE::EVENTFLAG, collided);
	GetCollisionMgr()->CheckCollision(OBJECT_TYPE::MONSTER, OBJECT_TYPE::EVENTFLAG, collided);

}

void TestScene::RenderScene()
{
}

void TestScene::PlayerSpawn()
{
	player = new Player(this, OBJECT_TYPE::PLAYER, 110);
	player->SetScale(1.5, 1.5);
	player->SetPos(testMap_1->GetPlayerStartPoint());
	
	CAMERA->SetTarget(player);
}

void TestScene::PlayerDieEvent()
{
    dialogUI->Clear();
	dialogUI->SetFace(DialogFace::Surprised);
	dialogUI->SetText(L"이런... 몸이 망가져버렸네요.\n곧 새로운 몸을 만들어 보내겠습니다.");
	dialogUI->IsWaiting(true);
	dialogUI->SetState(OBJECT_STATE::ACTIVE);
	dialogUI->SetUpdateEvent([this]() {
		if (KEYDOWN('F') && dialogUI->IsWait())
		{
			PlayerSpawn();
			testMap_1->SetPlayer(player);
			dialogUI->Clear();
			dialogUI->SetFace(DialogFace::Normal);
			dialogUI->SetText(L"이번에는 조심히 다뤄주세요.");
			dialogUI->IsWaiting(true);
			dialogUI->SetUpdateEvent([this]() {
				if (dialogUI->IsWait() && (KEYDOWN('F') || KEYPRESS(VK_LEFT) || KEYPRESS(VK_RIGHT) || KEYPRESS(VK_UP)))
				{
					dialogUI->Clear();
					dialogUI->SetState(OBJECT_STATE::HIDDEN);
					player->SetPreventKey(false);
				}
			});
		}
	});
}
