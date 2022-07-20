#include "Framework.h"
#include "AABB.h"

#include "Engine/Object/GameObject.h"

AABB::AABB(const D3DXVECTOR3& _min, const D3DXVECTOR3& _max, GameObject* _owner) :
	Component(_owner)
	, m_min(_min)
	, m_max(_max)
{
}

AABB::~AABB()
{
}

void AABB::Update()
{

}

void AABB::FinalUpdate()
{
}

void AABB::Render()
{
	RenderVertexWithoutTransform();
}

void AABB::OnUpdateWorldTransform()
{
	// min, max ÃÊ±âÈ­
	m_worldMin = m_min;
	m_worldMax = m_max;
	
	// Update Scale
	D3DXVECTOR3 scale = GetOwner()->GetScale();
	m_worldMin.x *= scale.x;
	m_worldMin.y *= scale.y;
	m_worldMin.z *= scale.z;
	m_worldMax.x *= scale.x;
	m_worldMax.y *= scale.y;
	m_worldMax.z *= scale.z;

	// Translate
	D3DXVECTOR3 pos = GetOwner()->GetPos();
	m_worldMin.x += pos.x;
	m_worldMin.y += pos.y;
	m_worldMin.z += pos.z;
	m_worldMax.x += pos.x;
	m_worldMax.y += pos.y;
	m_worldMax.z += pos.z;
}

void AABB::RenderVertexWithoutTransform()
{
	vector<VERTEXCOLOR> vertexList;
	vector<WORD>        indexList;

	D3DCOLOR color = 0xFF33FF33;

	vertexList.push_back(VERTEXCOLOR(m_worldMin.x, m_worldMin.y, color, -1.0f)); // LT
	vertexList.push_back(VERTEXCOLOR(m_worldMax.x, m_worldMin.y, color, -1.0f)); // RT
	vertexList.push_back(VERTEXCOLOR(m_worldMax.x, m_worldMax.y, color, -1.0f)); // RB
	vertexList.push_back(VERTEXCOLOR(m_worldMin.x, m_worldMax.y, color, -1.0f)); // LB
	
	//vertexList.push_back(VERTEXCOLOR(m_min.x, m_min.y, color, -1.0f)); // LT
	//vertexList.push_back(VERTEXCOLOR(m_max.x, m_min.y, color, -1.0f)); // RT
	//vertexList.push_back(VERTEXCOLOR(m_max.x, m_max.y, color, -1.0f)); // RB
	//vertexList.push_back(VERTEXCOLOR(m_min.x, m_max.y, color, -1.0f)); // LB

	indexList.push_back(0);
	indexList.push_back(1);
	indexList.push_back(2);
	indexList.push_back(3);
	indexList.push_back(0);

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	DEVICE->SetTransform(D3DTS_WORLD, &world);
	DEVICE->SetFVF(VERTEXCOLOR::FVF);
	DEVICE->DrawIndexedPrimitiveUP(D3DPT_LINESTRIP, 0, (UINT)vertexList.size(), (UINT)indexList.size() - 1, indexList.data(), D3DFMT_INDEX16, vertexList.data(), sizeof(VERTEXCOLOR));
}
