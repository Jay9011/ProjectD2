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
	testObject = new TestObject(GetGame(), this);
	testObjectPos = { 100, 100, 0 };
	testObjectSize = { 1, 1, 1 };
	TwAddVarRW(twbar, "Pos", TW_TYPE_DIR3F, &testObjectPos, "group=TestObject");
	TwAddVarRW(twbar, "Size", TW_TYPE_DIR3F, &testObjectSize, "group=TestObject");
	TwAddVarRW(twbar, "Angle", TW_TYPE_FLOAT, &testObjectAngle, "group=TestObject");
	
	testObject2 = new TestObject(GetGame(), this);
	testObject2->SetPos(512, 430, 0.5f);

	TestObject* tmpObject3 = new TestObject(GetGame(), this);
	tmpObject3->SetPos(1000, 600, 1.f);

	TestObject2* tmpObject4 = new TestObject2(GetGame(), this);
	tmpObject4->SetPos(512, 600, 0);
}

void ToolScene::Release()
{
}

void ToolScene::UpdateScene()
{
	testObject->SetPos(testObjectPos);
	testObject->SetScale(testObjectSize);
	testObject->SetAngle(testObjectAngle);
}

void ToolScene::RenderScene()
{
}
