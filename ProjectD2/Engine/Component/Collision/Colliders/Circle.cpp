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

bool Circle::Intersects(const D3DXVECTOR2& _point, OUT CollisionInfo& outColl)
{
	if(Contains(_point))
	{
		isCollided = true;
		return true;
	}
	
	return false;
}

bool Circle::Intersects(AARect* _rect, OUT CollisionInfo& outColl)
{
	return false;
}

bool Circle::Intersects(Circle* _circle, OUT CollisionInfo& outColl)
{
	return false;
}

void Circle::Update()
{
}

void Circle::FinalUpdate()
{
	// Collided 초기화
	isCollided = false;
}

void Circle::Render()
{
	if (!IsActive())
		return;

	RenderVertexWithoutTransform();
}

void Circle::OnUpdateWorldTransform()
{
	// worldCenter, worldRadius 초기화
	m_worldCenter = m_center;
	m_worldRadius = m_radius;

	// Scale에 따라서 worldRadius 계산 (x나 y 중에서 큰 값을 따른다) (타원이 아님...)
	D3DXVECTOR3 scale = GetOwner()->GetScale();
	m_worldRadius *= Math::Max(scale.x, scale.y);

	// Pos에 따라서 worldCenter 계산
	D3DXVECTOR3 pos = GetOwner()->GetPos();
	m_worldCenter += D3DXVECTOR2(pos.x, pos.y);
}

COLLIDER_TYPE Circle::GetColliderType()
{
	return COLLIDER_TYPE::CIRCLE;
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
		float x = m_radius *   cos(angle);
		float y = m_radius * - sin(angle);

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
