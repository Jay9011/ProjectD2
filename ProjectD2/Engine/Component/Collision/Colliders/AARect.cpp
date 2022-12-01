#include "Framework.h"
#include "AARect.h"

#include "Engine/Object/GameObject.h"
#include "Circle.h"

AARect::AARect(const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, class GameObject* _owner, int _updateOrder) :
	AARect(_min, _max, _owner->GetType(), _owner, _updateOrder)
{
}

AARect::AARect(const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, OBJECT_TYPE _type, GameObject* _owner, int _updateOrder) :
	Collider(_type, _owner, _updateOrder)
	, m_min(_min)
	, m_max(_max)
{
}

AARect::~AARect() = default;

float AARect::MinDistSq(const D3DXVECTOR2& _point)
{
	float dx = Math::Max(m_worldMin.x - _point.x, 0.0f);
	dx = Math::Max(dx, _point.x - m_worldMax.x);
	float dy = Math::Max(m_worldMin.y - _point.y, 0.0f);
	dy = Math::Max(dy, _point.y - m_worldMax.y);

	return dx * dx + dy * dy;
}

bool AARect::Contains(const D3DXVECTOR2& _point)
{
	bool outside = 
		_point.x < m_worldMin.x ||
		_point.y < m_worldMin.y ||
		_point.x > m_worldMax.x ||
		_point.y > m_worldMax.y;
	
	return !outside;
}

void AARect::Update()
{

}

void AARect::Render()
{
	if (!IsActive())
		return;
	
	if (Game::IsDbgRendering())
	{
		RenderVertex();
		RenderVertexWithoutTransform();
	}
}

void AARect::OnUpdateWorldTransform()
{
	// min, max 초기화
	m_worldMin = m_min;
	m_worldMax = m_max;
	
	GameObject* object = GetOwner();
	D3DXVECTOR2 worldScale = object->GetWorldScale();
	D3DXMATRIX worldMatrix = object->GetWorld();

	m_worldMin.x *= worldScale.x;
	m_worldMin.y *= worldScale.y;
	m_worldMax.x *= worldScale.x;
	m_worldMax.y *= worldScale.y;

	m_worldMin += worldMatrix.m[3];
	m_worldMax += worldMatrix.m[3];
	
	// Scale 반전에 의한 변경점 적용
	if (m_worldMin.x > m_worldMax.x) swap(m_worldMin.x, m_worldMax.x);
	if (m_worldMin.y > m_worldMax.y) swap(m_worldMin.y, m_worldMax.y);
}

COLLIDER_TYPE AARect::GetColliderType()
{
	return COLLIDER_TYPE::AARECT;
}

void AARect::RenderVertex()
{
	vector<VERTEXCOLOR> vertexList;
	vector<WORD>        indexList;

	D3DCOLOR color = isCollided ? onColor : offColor;

	vertexList.push_back(VERTEXCOLOR(m_min.x, m_min.y, color, -1.0f)); // LT
	vertexList.push_back(VERTEXCOLOR(m_max.x, m_min.y, color, -1.0f)); // RT
	vertexList.push_back(VERTEXCOLOR(m_max.x, m_max.y, color, -1.0f)); // RB
	vertexList.push_back(VERTEXCOLOR(m_min.x, m_max.y, color, -1.0f)); // LB

	indexList.push_back(0);
	indexList.push_back(1);
	indexList.push_back(2);
	indexList.push_back(3);
	indexList.push_back(0);

	DEVICE->SetFVF(VERTEXCOLOR::FVF);
	DEVICE->DrawIndexedPrimitiveUP(D3DPT_LINESTRIP, 0, (UINT)vertexList.size(), (UINT)indexList.size() - 1, indexList.data(), D3DFMT_INDEX16, vertexList.data(), sizeof(VERTEXCOLOR));
}

void AARect::RenderVertexWithoutTransform()
{
	vector<VERTEXCOLOR> vertexList;
	vector<WORD>        indexList;

	D3DCOLOR color = isCollided ? onColor : offColor;

	vertexList.push_back(VERTEXCOLOR(m_worldMin.x, m_worldMin.y, color, -1.0f)); // LT
	vertexList.push_back(VERTEXCOLOR(m_worldMax.x, m_worldMin.y, color, -1.0f)); // RT
	vertexList.push_back(VERTEXCOLOR(m_worldMax.x, m_worldMax.y, color, -1.0f)); // RB
	vertexList.push_back(VERTEXCOLOR(m_worldMin.x, m_worldMax.y, color, -1.0f)); // LB

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
	
	GetOwner()->SetWorld();
}
