#include "Framework.h"
#include "Transform.h"

Transform::Transform(Transform* _parent) :
	m_pos(0, 0, 0)
	, m_scale(1, 1, 1)
	, m_angle(0)
	, m_pivot(0, 0, 0)
	, m_recomputeWorld(true)
{
	D3DXMatrixIdentity(&m_S);
	D3DXMatrixIdentity(&m_R);
	D3DXMatrixIdentity(&m_T);
	D3DXMatrixIdentity(&m_world);
}

Transform::~Transform()
{
}

void Transform::UpdateWorld()
{
	if (m_recomputeWorld)
	{
		D3DXMatrixScaling(&m_S, m_scale.x, m_scale.y, m_scale.z);
		D3DXMatrixRotationZ(&m_R, m_angle);
		D3DXMatrixTranslation(&m_T, m_pos.x, m_pos.y, m_pos.z);

		D3DXMATRIX P;
		D3DXMATRIX IP;
		D3DXMatrixTranslation(&P, m_pivot.x, m_pivot.y, m_pivot.z);
		D3DXMatrixInverse(&IP, nullptr, &P);

		m_world = IP * m_S * m_R * m_T * P;

		m_recomputeWorld = false;
	}
}

void Transform::SetWorld()
{
	DEVICE->SetTransform(D3DTS_WORLD, &m_world);
}
