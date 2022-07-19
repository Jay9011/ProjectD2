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
	testObjectSize = { 2, 2, 1 };
	TwAddVarRW(twbar, "Size", TW_TYPE_DIR3F, &testObjectSize, "group=TestObject");
	TwAddVarRW(twbar, "Angle", TW_TYPE_FLOAT, &testObjectAngle, "group=TestObject");
}

void ToolScene::Release()
{
}

void ToolScene::UpdateScene()
{
	testObject->SetScale(testObjectSize);
	testObject->SetAngle(testObjectAngle);
}

void ToolScene::RenderScene()
{
}
