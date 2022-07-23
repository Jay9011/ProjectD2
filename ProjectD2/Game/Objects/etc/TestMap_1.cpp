#include "Framework.h"
#include "TestMap_1.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/Collision/Collision.h"

TestMap_1::TestMap_1(Scene* _scene, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, OBJECT_TYPE::PLATFORM, _updateOrder, _parent)
{
	Collider* tmpCol;

	tmpCol = ADDCOMP::NewAARect({ -300, -16 }, { 300, 16 }, this);
	tmpCol->IsActive(true);
	m_platforms.emplace_back(tmpCol);
	
	m_texture = TEXTURE->Add(L"Tile\\IndustrialTile_09.png", 600, 32, {0, 0}, {600, 32});
}

TestMap_1::~TestMap_1()
{
}

void TestMap_1::UpdateObject()
{
	
}

void TestMap_1::RenderObject()
{
	m_texture->Render();
}
