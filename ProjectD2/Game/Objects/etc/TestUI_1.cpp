#include "Framework.h"
#include "TestUI_1.h"

TestUI_1::TestUI_1(Scene* _scene, int _updateOrder, UIObject* _parent) :
    UIObject(_scene, _updateOrder, _parent)
{
}

TestUI_1::~TestUI_1()
{
}

void TestUI_1::UpdateObject()
{
}

void TestUI_1::RenderObject()
{
	vector<VERTEXCOLOR> vertexList;
	vector<WORD>        indexList;

	D3DCOLOR color = 0xFFF637EC;

	vertexList.push_back(VERTEXCOLOR(100.0f, 100.0f, color, 0.0f)); // LT
	vertexList.push_back(VERTEXCOLOR(300.0f, 100.0f, color, 0.0f)); // RT
	vertexList.push_back(VERTEXCOLOR(300.0f, 300.0f, color, 0.0f)); // RB
	vertexList.push_back(VERTEXCOLOR(100.0f, 300.0f, color, 0.0f)); // LB

	indexList.push_back(0);
	indexList.push_back(1);
	indexList.push_back(2);
	indexList.push_back(3);
	indexList.push_back(0);

	DEVICE->SetFVF(VERTEXCOLOR::FVF);
	DEVICE->DrawIndexedPrimitiveUP(D3DPT_LINESTRIP, 0, (UINT)vertexList.size(), (UINT)indexList.size() - 1, indexList.data(), D3DFMT_INDEX16, vertexList.data(), sizeof(VERTEXCOLOR));
}

void TestUI_1::SetVertex()
{
}
