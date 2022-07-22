#include "Framework.h"
#include "AARect.h"

#include "Engine/Object/GameObject.h"
#include "Circle.h"

AARect::AARect(const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, class GameObject* _owner, int _updateOrder) :
	Collider(_owner, _updateOrder)
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

void AARect::FinalUpdate()
{
	// Collided �ʱ�ȭ
	isCollided = false;
}

void AARect::RenderDebug()
{
	if (!IsActive())
		return;
	
	RenderVertexWithoutTransform();
}

void AARect::OnUpdateWorldTransform()
{
	// min, max �ʱ�ȭ
	m_worldMin = m_min;
	m_worldMax = m_max;
	
	// Update Scale
	D3DXVECTOR3 scale = GetOwner()->GetScale();
	m_worldMin.x *= scale.x;
	m_worldMin.y *= scale.y;
	m_worldMax.x *= scale.x;
	m_worldMax.y *= scale.y;

	// Translate
	D3DXVECTOR3 pos = GetOwner()->GetPos();
	m_worldMin.x += pos.x;
	m_worldMin.y += pos.y;
	m_worldMax.x += pos.x;
	m_worldMax.y += pos.y;

	// Scale ������ ���� ������ ����
	if (m_worldMin.x > m_worldMax.x) std::swap(m_worldMin.x, m_worldMax.x);
	if (m_worldMin.y > m_worldMax.y) std::swap(m_worldMin.y, m_worldMax.y);
}

COLLIDER_TYPE AARect::GetColliderType()
{
	return COLLIDER_TYPE::AARECT;
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
	
	GetOwner()->SetWorld();
}
