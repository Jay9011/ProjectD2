#include "Framework.h"
#include "Line.h"

#include "Engine/Object/GameObject.h"

Line::Line(const D3DXVECTOR2& _start, const D3DXVECTOR2& _end, GameObject* _owner, int _updateOrder) :
	Collider(_owner, _updateOrder)
	, m_start(_start)
	, m_end(_end)
{
}

Line::~Line() = default;

float Line::MinDistSq(const D3DXVECTOR2& _point)
{
	D3DXVECTOR2 ab = m_worldEnd - m_worldStart;
	D3DXVECTOR2 ba = -1.0f * ab;
	D3DXVECTOR2 ap = _point - m_worldStart;
	D3DXVECTOR2 bp = _point - m_worldEnd;
	
	if(D3DXVec2Dot(&ab, &ap) < 0.0f)
		return D3DXVec2LengthSq(&ap);
	if(D3DXVec2Dot(&ba, &bp) < 0.0f)
		return D3DXVec2LengthSq(&bp);
	
	float scalar = D3DXVec2Dot(&ap, &ab) / D3DXVec2Dot(&ab, &ab);
	D3DXVECTOR2 proj = scalar * ab;
	D3DXVECTOR2 vecP = ap - proj;
	return D3DXVec2LengthSq(&vecP);
}

float Line::MinDistSq(Line* _l1, Line* _l2)
{
	D3DXVECTOR2 u = _l1->m_worldEnd   - _l1->m_worldStart;
	D3DXVECTOR2 v = _l2->m_worldEnd   - _l2->m_worldStart;
	D3DXVECTOR2 w = _l1->m_worldStart - _l2->m_worldStart;

	float a = D3DXVec2Dot(&u, &u);
	float b = D3DXVec2Dot(&u, &v);
	float c = D3DXVec2Dot(&v, &v);
	float d = D3DXVec2Dot(&u, &w);
	float e = D3DXVec2Dot(&v, &w);
	float D = a * c - b * b;
	float sc, sN, sD = D;
	float tc, tN, tD = D;

	if (Math::NearZero(D))
	{
		sN = 0.0f;
		sD = 1.0f;
		tN = e;
		tD = c;
	}
	else
	{
		sN = (b * e - c * d);
		tN = (a * e - b * d);
		if (sN < 0.0f)
		{
			sN = 0.0f;
			tN = e;
			tD = c;
		}
		else if (sN > sD)
		{
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0)
	{
		tN = 0.0;
		if (-d < 0.0)
			sN = 0.0;
		else if (-d > a)
			sN = sD;
		else
		{
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD)
	{
		tN = tD;
		if ((-d + b) < 0.0)
			sN = 0;
		else if ((-d + b) > a)
			sN = sD;
		else
		{
			sN = (-d + b);
			sD = a;
		}
	}
	
	sc = (Math::NearZero(sN) ? 0.0f : sN / sD);
	tc = (Math::NearZero(tN) ? 0.0f : tN / tD);

	D3DXVECTOR2 dP = w + (sc * u) - (tc * v);

	return D3DXVec2LengthSq(&dP);
}

D3DXVECTOR2 Line::PointOnLine(float _t)
{
	return m_worldStart + (_t * (m_worldEnd - m_worldStart));
}

bool Line::Contains(const D3DXVECTOR2& _point)
{
	return Math::NearZero(MinDistSq(_point));
}

void Line::Update()
{
}

void Line::FinalUpdate()
{
	// Collided 초기화
	isCollided = false;
}

void Line::Render()
{
	if (!IsActive())
		return;

	RenderVertexWithoutTransform();
}

void Line::OnUpdateWorldTransform()
{
	m_worldStart = m_start;
	m_worldEnd   = m_end;

	// Scale에 따라 Start, End 길이 갱신
	D3DXVECTOR3 scale = GetOwner()->GetScale();
	m_worldStart.x *= scale.x;
	m_worldStart.y *= scale.y;
	m_worldEnd.x   *= scale.x;
	m_worldEnd.y   *= scale.y;

	D3DXVECTOR3 pos = GetOwner()->GetPos();
	m_worldStart.x += pos.x;
	m_worldStart.y += pos.y;
	m_worldEnd.x   += pos.x;
	m_worldEnd.y   += pos.y;
}

D3DXVECTOR2 Line::GetMin()
{
	return D3DXVECTOR2(Math::Min(m_worldStart.x, m_worldEnd.x), Math::Min(m_worldStart.y, m_worldEnd.y));
}

D3DXVECTOR2 Line::GetMax()
{
	return D3DXVECTOR2(Math::Max(m_worldStart.x, m_worldEnd.x), Math::Max(m_worldStart.y, m_worldEnd.y));
}

COLLIDER_TYPE Line::GetColliderType()
{
	return COLLIDER_TYPE::LINE;
}

void Line::RenderVertexWithoutTransform()
{
	vector<VERTEXCOLOR> vertices;
	
	D3DCOLOR color = isCollided ? onColor : offColor;

	vertices.push_back(VERTEXCOLOR(m_worldStart.x, m_worldStart.y, color, -1.0f));
	vertices.push_back(VERTEXCOLOR(m_worldEnd.x  , m_worldEnd.y  , color, -1.0f));

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	DEVICE->SetTransform(D3DTS_WORLD, &world);
	DEVICE->SetFVF(VERTEXCOLOR::FVF);
	DEVICE->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices.data(), sizeof(VERTEXCOLOR));

	GetOwner()->SetWorld();
}
