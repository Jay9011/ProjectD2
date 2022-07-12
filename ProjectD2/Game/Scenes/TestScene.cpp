#include "Framework.h"
#include "TestScene.h"

TestScene::TestScene(Game* _game) :
	Scene(_game)
{
	vertices[0].position = { 100, 100, 0 };
	vertices[0].color = 0xFFFF0000;
	vertices[1].position = { 200, 100, 0 };
	vertices[1].color = 0xFF00FF00;
	vertices[2].position = { 100, 200, 0 };
	vertices[2].color = 0xFF0000FF;
	vertices[3].position = { 200, 200, 0 };
	vertices[3].color = 0xFFFFFFFF;

}

TestScene::~TestScene()
{
}

void TestScene::Init()
{
}

void TestScene::Release()
{
}

void TestScene::Update()
{
}

void TestScene::Render()
{
	DEVICE->SetFVF(VertexColor::FVF);
	DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertices, sizeof(VertexColor));
}
