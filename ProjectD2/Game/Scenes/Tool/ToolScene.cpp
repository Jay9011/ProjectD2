#include "Framework.h"
#include "ToolScene.h"

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
}

void ToolScene::Release()
{
}

void ToolScene::UpdateScene()
{
}

void ToolScene::RenderScene()
{
}
