#include "Framework.h"
#include "TestScene.h"

#include "Game/Objects/etc/TestObject.h"

TestScene::TestScene(Game* _game) :
	Scene(_game)
{}

TestScene::~TestScene()
{}

void TestScene::Init()
{
	m_gameObj = new TestObject(this);
	m_gameObj->SetPos({ WIN_CENTER_X, WIN_CENTER_Y, 0 });
}

void TestScene::Release()
{
	SAFE_DELETE(m_gameObj);
}

void TestScene::UpdateScene()
{
}

void TestScene::RenderScene()
{
}