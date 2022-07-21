#include "Framework.h"
#include "ToolScene.h"

#include "Engine/Component/Collision/Collider.h"
#include "Game/Objects/etc/TestObject.h"
#include "Game/Objects/etc/TestObject2.h"

ToolScene::ToolScene(Game* _game) :
	Scene(_game)
{}

ToolScene::~ToolScene()
{}

void ToolScene::Init()
{
	testObject = new TestObject(this);
	testObject->SetPos({ WIN_CENTER_X, WIN_CENTER_Y, 0.5f });
	
	testObject2 = new TestObject2(this);
	testObject2->SetPos({ WIN_CENTER_X + 300, WIN_CENTER_Y, 0.0f });
}

void ToolScene::Release()
{
}

void ToolScene::UpdateScene()
{
	CollisionInfo info = {};
	if (testObject->m_bodyCollider->Intersects(testObject2->m_bodyCollider, info))
	{
		info.other->IsCollided(true);
	}
}

void ToolScene::RenderScene()
{
}
