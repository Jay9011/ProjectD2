#include "Framework.h"
#include "TestMap_1.h"

#include "Engine/Object/GameObject.h"
#include "Engine/Resource/Shader.h"
#include "Engine/Component/Collision/Colliders/AARect.h"
#include "Game/Objects/Background/Background.h"
#include "Game/Objects/Platforms/PlatformRect.h"
#include "Game/Objects/Monster/MonsterFactory.h"

TestMap_1::TestMap_1(Scene* _scene, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, OBJECT_TYPE::DEFAULT, _updateOrder, _parent)
	, m_scene(_scene)
{
	/*
	* Ä«¸Þ¶ó ¼³Á¤
	*/
	//CAMERA->SetRestrictRange(0, 0, 1300, 1080);
	CAMERA->SetSpeed(1500.0f);
    
    /*
	* Create Background
	*/
	Background* background = nullptr;
    
	background = new Background(TEXTURE->Add(L"Background\\2.png"), SHADER(L"AlphaShader"), { -0.6f, -0.6f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(-350, 20);
	m_backgrounds.emplace_back(background);
    
	background = new Background(TEXTURE->Add(L"Background\\2.png"), SHADER(L"AlphaShader"), { -0.6f, -0.6f }, _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(-350 + background->LocalSize().x, 20);
	m_backgrounds.emplace_back(background);
    
	background = new Background(TEXTURE->Add(L"Background\\3.png"), SHADER(L"AlphaShader"), { -0.3f, -0.5f } , _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0, 100);
	m_backgrounds.emplace_back(background);
    
	background = new Background(TEXTURE->Add(L"Background\\3.png"), SHADER(L"AlphaShader"), { -0.3f, -0.5f } , _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0 - background->LocalSize().x, 100);
	m_backgrounds.emplace_back(background);
    
	background = new Background(TEXTURE->Add(L"Background\\4.png"), SHADER(L"AlphaShader"), { -0.1f, -0.3f } , _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0 - background->LocalSize().x, 150);
	m_backgrounds.emplace_back(background);
    
	background = new Background(TEXTURE->Add(L"Background\\4.png"), SHADER(L"AlphaShader"), { -0.1f, -0.3f } , _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0, 150);
	m_backgrounds.emplace_back(background);
    
	background = new Background(TEXTURE->Add(L"Background\\5.png"), SHADER(L"AlphaShader"), { 0.2f, -0.1f } , _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0 - background->LocalSize().x, 200);
	m_backgrounds.emplace_back(background);
	
	background = new Background(TEXTURE->Add(L"Background\\5.png"), SHADER(L"AlphaShader"), { 0.2f, -0.1f } , _scene, OBJECT_TYPE::BACKGROUND, _updateOrder, this);
	background->SetScale(1.5f, 1.5f);
	background->SetPos(0, 200);
	m_backgrounds.emplace_back(background);

	/*
	* Create platforms
    */
	PlatformRect* platform;

	m_PlayerStartFlag = new PlatformRect({ 10, 10 }, false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, this);
	m_PlayerStartFlag->SetPos(-336, 152);

	platform = new PlatformRect({640, 32}, L"Tile\\IndustrialTile_32.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(0, 192);
	platform->GetCollider()->SetTag("Floor");
	m_platforms.push_back(platform);
	
	platform = new PlatformRect({32, 32}, L"Tile\\IndustrialTile_33.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(336, 192);
	m_platforms.push_back(platform);
	
	platform = new PlatformRect({640, 32}, L"Tile\\IndustrialTile_41.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(0, 224);
	m_platforms.push_back(platform);

	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_40.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(-336, 224);
	m_platforms.push_back(platform);

	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_42.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(336, 224);
	m_platforms.push_back(platform);

#pragma region ¿ÞÂÊ º® ¼¼Æ®
	// ÇÏ´Ü ¿ì
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_16.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(-368, 192);
	// Áß¾Ó ¿ì
	platform = new PlatformRect({ 32, 448 }, L"Tile\\IndustrialTile_15.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -240);
	platform->GetCollider()->options.slidable = true;
	platform->GetCollider()->options.resistance.y = 10.0f;
	// »ó´Ü ¿ì
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_06.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -240);
	platform->GetCollider()->options.slidable = true;
	platform->GetCollider()->options.resistance.y = 10.0f;
	// »ó´Ü Áß
	platform = new PlatformRect({ 64, 32 }, L"Tile\\IndustrialTile_05.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(-48, 0);
	// Áß¾Ó µ¢¾î¸®
	platform = new PlatformRect({ 64, 448 }, L"Tile\\IndustrialTile_14.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, 240);
	// ÇÏ´Ü Áß
	platform = new PlatformRect({ 64, 32 }, L"Tile\\IndustrialTile_14.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, 240);
    // ÇÏ´Ü ÁÂ
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_12.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(-48, 0);
	// Áß¾Ó ÁÂ
	platform = new PlatformRect({ 32, 448 }, L"Tile\\IndustrialTile_13.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -240);
	// »ó´Ü ÁÂ
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_04.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(0, -240);
#pragma endregion

	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_31.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(-336, 192);
	platform->GetCollider()->SetTag("LeftFloor");
	m_platforms.push_back(platform);
	
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_14.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(-368, 224);
	m_platforms.push_back(platform);
	
	/*
	* Object Placement
	*/
	platform = new PlatformRect({32, 64}, L"Tile\\Entry.png", 8, 1, false, _scene, OBJECT_TYPE::FOREBLOCK, _updateOrder, this);
	platform->SetPos(336, 144);

    /*
	*/
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
	MonsterFactory::CreateMonster(m_scene, MONSTERS::MMM, { 0, 0 }, this);
}
