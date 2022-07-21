#pragma once
#include "Engine/Component/Collision/Collider.h"

class AARect : public Collider
{
public:
	AARect(const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, class GameObject* _owner, int _updateOrder = 100);
	virtual ~AARect() override;

public:
	float MinDistSq(const D3DXVECTOR2& _point);
	
	// Collider을(를) 통해 상속됨
	virtual bool Contains(const D3DXVECTOR2& _point) override;

	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void OnUpdateWorldTransform() override;
	virtual void Render() override;

	
private:
	void RenderVertexWithoutTransform();

private:
	D3DXVECTOR2 m_min;
	D3DXVECTOR2 m_max;
	D3DXVECTOR2 m_worldMin;
	D3DXVECTOR2 m_worldMax;

/* === === === === ===
*   Getter / Setter
* === === === === === */
public:
	virtual COLLIDER_TYPE GetColliderType() override;

	void SetMin(const D3DXVECTOR2& _min) { m_min = _min; }
	void SetMax(const D3DXVECTOR2& _max) { m_max = _max; }
	
	D3DXVECTOR2 GetMin() override { return m_worldMin; };
	D3DXVECTOR2 GetMax() override { return m_worldMax; };

};

