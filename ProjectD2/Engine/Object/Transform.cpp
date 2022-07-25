#include "Framework.h"
#include "Transform.h"

Transform::Transform(Transform* _parent) :
	m_parent(_parent)
	, m_pos(0, 0)
	, m_scale(1, 1)
	, m_angle(0)
	, m_pivot(0, 0)
	, m_recomputeWorld(true)
	, m_sumParentsPos(m_pos)
	, m_sumParentsScale(m_scale)
	, m_sumParentsAngle(m_angle)
	, m_sumParentsPivot(m_pivot)
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
		D3DXMatrixScaling(&m_S, m_scale.x, m_scale.y, 1);
		D3DXMatrixRotationZ(&m_R, m_angle);
		D3DXMatrixTranslation(&m_T, m_pos.x, m_pos.y, 0);

		D3DXMATRIX P;
		D3DXMATRIX IP;
		D3DXMatrixTranslation(&P, m_pivot.x, m_pivot.y, 0);
		D3DXMatrixInverse(&IP, nullptr, &P);

		m_world = IP * m_S * m_R * m_T * P;

		if (m_parent != nullptr)
		{
			m_world *= GetParent()->GetWorld();
			SumParentValues();
		}
		
		m_recomputeWorld = false;
	}
}

void Transform::SetWorld()
{
	DEVICE->SetTransform(D3DTS_WORLD, &m_world);
}

void Transform::SumParentValues()
{
	if (m_parent == nullptr)
		return;
	
	m_sumParentsPos   = m_world.m[3];
	m_sumParentsScale = m_parent->GetWorldScale();
	m_sumParentsAngle = m_parent->GetWorldAngle();
	m_sumParentsPivot = m_parent->GetWorldPivot();
}
