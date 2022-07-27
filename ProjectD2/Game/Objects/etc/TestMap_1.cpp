#include "Framework.h"
#include "TestMap_1.h"

#include "Engine/Object/GameObject.h"
#include "Engine/Resource/Shader.h"
#include "Engine/Component/Collision/Colliders/AARect.h"
#include "Game/Objects/Platforms/PlatformRect.h"

TestMap_1::TestMap_1(Scene* _scene, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, OBJECT_TYPE::DEFAULT, _updateOrder, _parent)
{
	m_backgrounds.emplace_back(TEXTURE->Add(L"Background\\2.png", WIN_WIDTH, WIN_HEIGHT));
	m_backgrounds.emplace_back(TEXTURE->Add(L"Background\\3.png", WIN_WIDTH * 0.9, WIN_HEIGHT * 0.9));
	m_backgrounds.emplace_back(TEXTURE->Add(L"Background\\4.png", WIN_WIDTH * 0.8, WIN_HEIGHT * 0.8));
	m_backgrounds.emplace_back(TEXTURE->Add(L"Background\\5.png", WIN_WIDTH * 0.7, WIN_HEIGHT * 0.7));
	m_backgroundShader = SHADER(L"AlphaShader");
	
	PlatformRect* platform;

	m_PlayerStartFlag = new PlatformRect({ 10, 10 }, false, _scene, OBJECT_TYPE::DEFAULT, _updateOrder, this);
	m_PlayerStartFlag->SetPos(-336, 152);

	platform = new PlatformRect({640, 32}, L"Tile\\IndustrialTile_32.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(0, 192);
	platform->GetCollider()->SetTag("Floor");
	m_platforms.push_back(platform);
	
	platform = new PlatformRect({32, 32}, L"Tile\\IndustrialTile_31.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(-336, 192);
	platform->GetCollider()->SetTag("LeftFloor");
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

	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_16.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(-368, 192);
	m_platforms.push_back(platform);
	
	platform = new PlatformRect({ 32, 224 }, L"Tile\\IndustrialTile_15.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(-368, -160);
	platform->GetCollider()->SetTag("Left Wall1");
	platform->GetCollider()->options.slidable = true;
	platform->GetCollider()->options.resistance.y = 10.0f;
	m_platforms.push_back(platform);
	
	platform = new PlatformRect({ 32, 224 }, L"Tile\\IndustrialTile_15.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(-368, 64);
	platform->GetCollider()->SetTag("Left Wall2");
	platform->GetCollider()->options.slidable = true;
	platform->GetCollider()->options.resistance.y = 10.0f;
	m_platforms.push_back(platform);
	
	platform = new PlatformRect({ 64, 480 }, L"Tile\\IndustrialTile_14.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(-416, 0);
	m_platforms.push_back(platform);
	
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_14.png", false, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(-368, 224);
	m_platforms.push_back(platform);
	
	platform = new PlatformRect({ 64, 32 }, L"Tile\\IndustrialTile_59.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	m_testPlatform = platform;
	
	m_platforms.push_back(platform);

#if _DEBUG
	m_pos = { 0, 0 };
	m_scale = { 1, 1 };
	TwAddVarRW(_scene->twbar, "Pos", TW_TYPE_DIR3F, &m_pos, "");
	TwAddVarRW(_scene->twbar, "Scale", TW_TYPE_DIR3F, &m_scale, "min=0.0 max=100.0");
#endif // _DEBUG
	
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_59.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(48, 32);
	platform->SetScale(1.5, 1.5);
	
	platform = new PlatformRect({ 32, 32 }, L"Tile\\IndustrialTile_59.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, platform);
	platform->SetPos(32, 32);
	platform->SetScale(1.5, 1.5);
	
	platform = new PlatformRect({32, 64}, L"Tile\\Entry.png", 8, 1, false, _scene, OBJECT_TYPE::FOREBLOCK, _updateOrder, this);
	platform->SetPos(336, 144);

}

TestMap_1::~TestMap_1()
{
}

void TestMap_1::UpdateObject()
{
	if (KEYDOWN(VK_F8))
	{
		m_Entry->AnimPauseOrPlay();
	}

	m_testPlatform->SetPos(m_pos);
	m_testPlatform->SetScale(m_scale);
}

void TestMap_1::RenderObject()
{
	m_backgroundShader->Begin();
	for (auto& background : m_backgrounds)
	{
		background->Render();
	}
	m_backgroundShader->End();
	
	GameObject::RenderObject();
}

D3DXVECTOR2 TestMap_1::GetPlayerStartPoint()
{
	assert(m_PlayerStartFlag);

	D3DXVECTOR2 pos = m_PlayerStartFlag->GetWorldPos();
	
	return pos;
}
