#include "Framework.h"
#include "TestScene.h"

#include "Game/Objects/etc/TestObject.h"

TestScene::TestScene(Game* _game) :
	Scene(_game)
{

	m_gameObj = new TestObject(_game, this);
	m_gameObj->SetPos({ WIN_CENTER_X, WIN_CENTER_Y, 0 });
}

TestScene::~TestScene()
{
	SAFE_DELETE(m_gameObj);
}

void TestScene::Init()
{
}

void TestScene::Release()
{
}

void TestScene::UpdateScene()
{
}

void TestScene::RenderScene()
{
}