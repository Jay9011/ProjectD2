#pragma once
#include "Engine/Component/Component.h"

class AABB : public Component
{
public:
	AABB(const D3DXVECTOR3& _min, const D3DXVECTOR3& _max, GameObject* _owner = nullptr);
	virtual ~AABB();

public:
	// Component을(를) 통해 상속됨
	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void Render() override;
	virtual void OnUpdateWorldTransform() override;
	
private:
	D3DXVECTOR3 m_min;
	D3DXVECTOR3 m_max;
	D3DXVECTOR3 m_worldMin;
	D3DXVECTOR3 m_worldMax;

private:
	void RenderVertexWithoutTransform();
};

