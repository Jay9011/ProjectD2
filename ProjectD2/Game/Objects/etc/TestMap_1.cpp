#include "Framework.h"
#include "TestMap_1.h"

#include "Engine/Object/GameObject.h"
#include "Engine/Resource/Shader.h"
#include "Engine/Component/Collision/Colliders/AARect.h"
#include "Game/Objects/Background/Background.h"
#include "Game/Objects/Platforms/PlatformRect.h"
#include "Game/Objects/Monster/MonsterFactory.h"
#include "Game/Objects/Interactive/ScreenButton.h"
#include "Game/Objects/Interactive/Door.h"

TestMap_1::TestMap_1(Scene* _scene, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, OBJECT_TYPE::DEFAULT, _updateOrder, _parent)
	, m_scene(_scene)
{
	/*
	* 카메라 설정
	*/
	//CAMERA->SetRestrictRange(-80.0f, -1000.0f, FLT_MAX, FLT_MAX);
	CAMERA->SetRestrictRange(-80.0f, -1000.0f, 1264.0f, FLT_MAX);
	CAMERA->SetSpeed(1500.0f);
    
	/*
	* Create Background
	*/
	Background* background = nullptr;

#pragma region 배경 설정
	background = new Background(TEXTURE->Add(L"Background\\2.png"), SHADER(L"AlphaShader"), { -0.6f, -0.6f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(-350, 20);
	m_backgrounds.emplace_back(background);

	background = new Background(TEXTURE->Add(L"Background\\2.png"), SHADER(L"AlphaShader"), { -0.6f, -0.6f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(-350 + background->LocalSize().x, 20);
	m_backgrounds.emplace_back(background);

	background = new Background(TEXTURE->Add(L"Background\\3.png"), SHADER(L"AlphaShader"), { -0.3f, -0.5f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0, 100);
	m_backgrounds.emplace_back(background);

	background = new Background(TEXTURE->Add(L"Background\\3.png"), SHADER(L"AlphaShader"), { -0.3f, -0.5f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0 - background->LocalSize().x, 100);
	m_backgrounds.emplace_back(background);

	background = new Background(TEXTURE->Add(L"Background\\4.png"), SHADER(L"AlphaShader"), { -0.1f, -0.3f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0 - background->LocalSize().x, 150);
	m_backgrounds.emplace_back(background);

	background = new Background(TEXTURE->Add(L"Background\\4.png"), SHADER(L"AlphaShader"), { -0.1f, -0.3f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0, 150);
	m_backgrounds.emplace_back(background);

	background = new Background(TEXTURE->Add(L"Background\\5.png"), SHADER(L"AlphaShader"), { 0.2f, -0.1f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0 - background->LocalSize().x, 200);
	m_backgrounds.emplace_back(background);

	background = new Background(TEXTURE->Add(L"Background\\5.png"), SHADER(L"AlphaShader"), { 0.2f, -0.1f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0, 200);
	m_backgrounds.emplace_back(background);
#pragma endregion

	/*
	* Create platforms
    */
	PlatformRect *platform, *platformLT, *platformMT, *platformRT, *platformLM, *platformMM, *platformRM, *platformLB, *platformMB, *platformRB;

#pragma region 왼쪽 벽 세트
	// 우하단
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_16.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platformRB->SetPos(-368, 192);
	// 중하단
	platformMB = new PlatformRect({ 64, 32 }, L"Tile\\IndustrialTile_14.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformMB->SetPos(-48, 0);
	// 좌하단
	platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_12.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformLB->SetPos(-48, 0);
	// 우중단
	platformRM = new PlatformRect({ 32, 448 }, L"Tile\\IndustrialTile_15.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformRM->SetPos(0, -240);
	platformRM->GetCollider()->options.slidable = true;
	platformRM->GetCollider()->options.resistance.y = 10.0f;
	// 중중단
	platformMM = new PlatformRect({ 64, 448 }, L"Tile\\IndustrialTile_14.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformMM->SetPos(0, -240);
	// 좌중단
	platformLM = new PlatformRect({ 32, 448 }, L"Tile\\IndustrialTile_13.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformLM->SetPos(0, -240);
	// 우상단
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_06.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRT->SetPos(0, -240);
	platformRT->GetCollider()->options.slidable = true;
	platformRT->GetCollider()->options.resistance.y = 10.0f;
	// 중상단
	platformMT = new PlatformRect({ 64, 32 }, L"Tile\\IndustrialTile_05.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMT->SetPos(0, -240);
	// 좌상단
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_04.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLM);
	platformLT->SetPos(0, -240);
#pragma endregion
	platform = new PlatformRect({ 128, 320 }, L"Tile\\IndustrialTile_02.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platform->SetPos(0, 176);

	// 굴뚝
	platform = new PlatformRect({ 32, 160 }, L"Tile\\IndustrialTile_63.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platform->SetPos(32, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_54.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_45.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -32);

#pragma region 아래쪽 바닥 세트 1
	// 좌상단
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_31.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platformLT->SetPos(-336, 192);
	platformLT->GetCollider()->SetTag("LeftFloor");
	// 상단
	platformMT = new PlatformRect({ 704, 32 }, L"Tile\\IndustrialTile_32.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platformMT->SetPos(368, 0);
	platformMT->GetCollider()->SetTag("Floor");
	// 우상단
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_33.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMT);
	platformRT->SetPos(368, 0);
	platformRT->GetCollider()->options.slidable = true;
	platformRT->GetCollider()->options.resistance.y = 10.0f;
	// 좌중간
	platformLM = new PlatformRect({ 32, 384 }, L"Tile\\IndustrialTile_40.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platformLM->SetPos(0, 208);
	// 중간
	platformMM = new PlatformRect({ 704, 384 }, L"Tile\\IndustrialTile_41.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMT);
	platformMM->SetPos(0, 208);
	// 우중단
	platformRM = new PlatformRect({ 32, 224 }, L"Tile\\IndustrialTile_42.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRT);
	platformRM->SetPos(0, 128);
	platformRM->GetCollider()->options.slidable = true;
	platformRM->GetCollider()->options.resistance.y = 10.0f;
	platformRM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_43.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRM->SetPos(0, 128);
	platformRM = new PlatformRect({ 32, 128 }, L"Tile\\IndustrialTile_41.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRM->SetPos(0, 80);
	// 좌하단
	platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_49.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLM);
	platformLB->SetPos(0, 208);
	// 중하단
	platformMB = new PlatformRect({ 704, 32 }, L"Tile\\IndustrialTile_50.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMB->SetPos(0, 208);
	// 우하단
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_50.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRB->SetPos(0, 80);
#pragma endregion

#pragma region 아래쪽 바닥 세트 2
	// 좌하단
	platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_50.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformLB->SetPos(32, 0);
	// 중하단
	platformMB = new PlatformRect({ 1408, 32 }, L"Tile\\IndustrialTile_50.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformMB->SetPos(720, 0);
	// 우하단
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_51.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformRB->SetPos(720, 0);
	platformRB->GetCollider()->options.slidable = true;
	platformRB->GetCollider()->options.resistance.y = 10.0f;
	// 좌중간
	platformLM = new PlatformRect({ 32, 128 }, L"Tile\\IndustrialTile_41.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformLM->SetPos(0, -80);
	// 중간
	platformMM = new PlatformRect({ 1408, 128 }, L"Tile\\IndustrialTile_41.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformMM->SetPos(0, -80);
	// 우중단
	platformRM = new PlatformRect({ 32, 128 }, L"Tile\\IndustrialTile_42.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformRM->SetPos(0, -80);
	platformRM->GetCollider()->options.slidable = true;
	platformRM->GetCollider()->options.resistance.y = 10.0f;
	// 좌상단
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_32.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLM);
	platformLT->SetPos(0, -80);
	// 상단
	platformMT = new PlatformRect({ 1408, 32 }, L"Tile\\IndustrialTile_32.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMT->SetPos(0, -80);
	// 우상단
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_33.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRT->SetPos(0, -80);
	platformRT->GetCollider()->options.slidable = true;
	platformRT->GetCollider()->options.resistance.y = 10.0f;
#pragma endregion

#pragma region 우측 벽 세트
	// 좌하단
	platformLB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_36.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformLB->SetPos(32, 0);
	// 중하단
	platformMB = new PlatformRect({ 160, 32 }, L"Tile\\IndustrialTile_36.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformMB->SetPos(96, 0);
	// 우하단
	platformRB = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_36.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformRB->SetPos(96, 0);
	// 좌중단
	platformLM = new PlatformRect({ 32, 480 }, L"Tile\\IndustrialTile_13.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformLM->SetPos(0, -256);
	platformLM->GetCollider()->options.slidable = true;
	platformLM->GetCollider()->options.resistance.y = 10.0f;
	// 중중단
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_28.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLB);
	platformMM->SetPos(32, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_19.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_10.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_28.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_19.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_10.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_28.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_19.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_10.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_28.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_19.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_10.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_28.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_19.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_10.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMM->SetPos(0, -32);
	platformMM = new PlatformRect({ 128, 480 }, L"Tile\\IndustrialTile_14.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMB);
	platformMM->SetPos(16, -256);
	// 우중단
	platformRM = new PlatformRect({ 32, 480 }, L"Tile\\IndustrialTile_15.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRB);
	platformRM->SetPos(0, -256);
	// 좌상단
	platformLT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_04.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLM);
	platformLT->SetPos(0, -256);
	platformRT->GetCollider()->options.slidable = true;
	platformRT->GetCollider()->options.resistance.y = 10.0f;
	// 중상단
	platformMT = new PlatformRect({ 160, 32 }, L"Tile\\IndustrialTile_05.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformMM);
	platformMT->SetPos(-16, -256);
	// 우상단
	platformRT = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_06.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRM);
	platformRT->SetPos(0, -256);
#pragma endregion
	
	// 굴뚝
	platform = new PlatformRect({ 32, 160 }, L"Tile\\IndustrialTile_63.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformLT);
	platform->SetPos(32, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_54.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_45.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -32);

	// 굴뚝
	platform = new PlatformRect({ 32, 160 }, L"Tile\\IndustrialTile_63.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platformRT);
	platform->SetPos(-32, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_54.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -96);
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_45.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -32);

	
	/*
	* Object Placement
	*/
    Door* door = new Door(_scene, OBJECT_TYPE::FOREBLOCK, _updateOrder, this);
	door->SetPos(400, 144);
	door->SetOpenFunc([](){
		CAMERA->SetRestrictRange(-80.0f, -1000.0f, FLT_MAX, FLT_MAX);
	});
    
	ScreenButton* button = new ScreenButton(_scene, OBJECT_TYPE::INTERACTIVE, _updateOrder, this);
	button->SetPos(300, 155);
	button->SetInteractObject(door);

    platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_71.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, door);
	platform->SetPos(0, -48);
    platform = new PlatformRect({ 32, 320 }, L"Tile\\IndustrialTile_73.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -176);

    /*
	*/
	m_PlayerStartFlag = new PlatformRect({ 10, 10 }, false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, this);
	m_PlayerStartFlag->SetPos(-336, 152);

	SetMonsters();
}

TestMap_1::~TestMap_1()
{
}

void TestMap_1::CameraInit()
{
	for (auto& bg : m_backgrounds)
	{
		bg->InitCameraPos();
	}
}

void TestMap_1::UpdateObject()
{
	for (auto& bg : m_backgrounds)
	{
		D3DXVECTOR2 camDir = bg->GetCameraDir();
		if (camDir.x == 0 && camDir.y == 0)
            continue;
        
		if (camDir.x > 0 && bg->Right() < CAMERA->Left())
		{
			bg->AddPos((bg->LocalSize().x * 2.0f), 0.0f);
		}
        else if (camDir.x < 0 && bg->Left() > CAMERA->Right())
        {
            bg->AddPos(-(bg->LocalSize().x * 2.0f), 0.0f);
        }
	}
}

void TestMap_1::RenderObject()
{
	GameObject::RenderObject();
}

D3DXVECTOR2 TestMap_1::GetPlayerStartPoint()
{
	assert(m_PlayerStartFlag);

	D3DXVECTOR2 pos = m_PlayerStartFlag->GetWorldPos();
	
	return pos;
}

void TestMap_1::SetMonsters()
{
	MonsterFactory::CreateMonster(m_scene, MONSTERS::MMM, { 0, 130 }, this);
    
	MonsterFactory::CreateMonster(m_scene, MONSTERS::MMM, { 600, 300 }, this);
	MonsterFactory::CreateMonster(m_scene, MONSTERS::MMM, { 750, 300 }, this);
	MonsterFactory::CreateMonster(m_scene, MONSTERS::MMM, { 1000, 300 }, this);
}
