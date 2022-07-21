#include "Framework.h"
#include "ToolScene.h"

#include "Engine/Component/Collision/Collision.h"
#include "Game/Objects/etc/TestObject.h"
#include "Game/Objects/etc/TestObject2.h"
#include "Game/Objects/etc/TestObjectCircle.h"

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

	testObjectCircle = new TestObjectCircle(this);
	testObjectCircle->SetPos({ WIN_CENTER_X - 300, WIN_CENTER_Y, 0.0f });
	testObjectCircle->SetScale(5.0f, 5.0f);
}

void ToolScene::Release()
{
}

void ToolScene::UpdateScene()
{
	if (Collision(testObject->m_bodyCollider, testObject2->m_bodyCollider))
	{
	}
	
	if (Collision(testObject->m_bodyCollider, testObjectCircle->m_bodyCollider))
	{
	}
	
	Collision(testObjectCircle->m_bodyCollider, MOUSEPOS);
}

void ToolScene::RenderScene()
{
}
