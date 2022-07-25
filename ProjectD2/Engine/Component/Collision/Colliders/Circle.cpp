#include "Framework.h"
#include "Circle.h"

#include "Engine/Object/GameObject.h"
#include "AARect.h"

Circle::Circle(const D3DXVECTOR2& center, const float& radius, GameObject* _owner, int _updateOrder) :
	Collider(_owner, _updateOrder)
	, m_center(center)
	, m_radius(radius)
{
}

Circle::~Circle() = default;

bool Circle::Contains(const D3DXVECTOR2& _point)
{
	D3DXVECTOR2 diff = m_worldCenter - _point;
	float distSq = D3DXVec2LengthSq(&diff);

	return distSq <= GetRadiusSq();
}

void Circle::Update()
{
}

void Circle::Render()
{
	if (!IsActive())
		return;

	if (Game::IsDbgRendering())
	{
		RenderVertex();
		RenderVertexWithoutTransform();
	}
}

void Circle::OnUpdateWorldTransform()
{
	// worldCenter, worldRadius ÃÊ±âÈ­
	m_worldCenter = m_center;
	m_worldRadius = m_radius;

	GameObject* object = GetOwner();
	D3DXVECTOR2 worldScale = GetOwner()->GetWorldScale();
	D3DXMATRIX worldMatrix = object->GetWorld();
	
	m_worldRadius *= Math::Max(worldScale.x, worldScale.y);
	m_worldCenter += worldMatrix.m[3];
}

COLLIDER_TYPE Circle::GetColliderType()
{
	return COLLIDER_TYPE::CIRCLE;
}

void Circle::RenderVertex()
{
	vector<VERTEXCOLOR> vertexList;
	vector<WORD>        indexList;

	D3DCOLOR color = isCollided ? onColor : offColor;

	short sectorCount = 36;

	float oneSector = 2 * PI / sectorCount;

	for (int i = 0; i < sectorCount; i++)
	{
		float angle = (float)i * oneSector;
		float x = m_radius * cos(angle);
		float y = m_radius * -sin(angle);

		vertexList.push_back(VERTEXCOLOR(x + m_center.x, y + m_center.y, color, -1.0f));
		indexList.push_back((WORD)i);
	}
	indexList.push_back(0);

	DEVICE->SetFVF(VERTEXCOLOR::FVF);
	DEVICE->DrawIndexedPrimitiveUP(D3DPT_LINESTRIP, 0, (UINT)vertexList.size(), (UINT)indexList.size() - 1, indexList.data(), D3DFMT_INDEX16, vertexList.data(), sizeof(VERTEXCOLOR));
}

void Circle::RenderVertexWithoutTransform()
{
	vector<VERTEXCOLOR> vertexList;
	vector<WORD>        indexList;

	D3DCOLOR color = isCollided ? onColor : offColor;

	short sectorCount = 36;
	
	float oneSector = 2 * PI / sectorCount;

	for (int i = 0; i < sectorCount; i++)
	{
		float angle = (float)i * oneSector;
		float x = m_worldRadius *   cos(angle);
		float y = m_worldRadius * - sin(angle);

		vertexList.push_back(VERTEXCOLOR(x + m_worldCenter.x, y + m_worldCenter.y, color, -1.0f));
		 indexList.push_back((WORD)i);
	}
	indexList.push_back(0);

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	DEVICE->SetTransform(D3DTS_WORLD, &world);
	DEVICE->SetFVF(VERTEXCOLOR::FVF);
	DEVICE->DrawIndexedPrimitiveUP(D3DPT_LINESTRIP, 0, (UINT)vertexList.size(), (UINT)indexList.size() - 1, indexList.data(), D3DFMT_INDEX16, vertexList.data(), sizeof(VERTEXCOLOR));
	
	GetOwner()->SetWorld();
}
