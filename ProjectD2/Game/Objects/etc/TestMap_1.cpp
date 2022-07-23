#include "Framework.h"
#include "TestMap_1.h"

#include "Engine/Object/GameObject.h"
#include "Engine/Component/Component.h"
#include "Engine/Component/Collision/Collision.h"
#include "Game/Objects/Platforms/PlatformRect.h"

TestMap_1::TestMap_1(Scene* _scene, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, OBJECT_TYPE::PLATFORM, _updateOrder, _parent)
{
	PlatformRect* platform = new PlatformRect({64, 64}, L"Tile\\Entry.png", 8, 1, true, _scene, OBJECT_TYPE::FOREBLOCK, _updateOrder, this);
	platform->SetPos(100, 100);
	m_platforms.push_back(platform);
	
	platform = new PlatformRect({640, 32}, L"Tile\\IndustrialTile_09.png", true, _scene, OBJECT_TYPE::PLATFORM, _updateOrder, this);
	platform->SetPos(100, 100);
	m_platforms.push_back(platform);
}

TestMap_1::~TestMap_1()
{
}

void TestMap_1::UpdateObject()
{
	
}

void TestMap_1::RenderObject()
{
	GameObject::RenderObject();
}
