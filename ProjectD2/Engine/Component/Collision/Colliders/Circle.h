#pragma once
#include "Engine/Component/Collision/Collider.h"

class Circle : public Collider
{
public:
	Circle(const D3DXVECTOR2& center, const float& radius, class GameObject* _owner, int _updateOrder = 100);
	virtual ~Circle() override;

public:
	// Collider을(를) 통해 상속됨
	virtual bool Contains(const D3DXVECTOR2& _point) override;
	
	virtual bool Intersects(const D3DXVECTOR2& _point, OUT CollisionInfo& outColl) override;
	virtual bool Intersects(AARect* _rect            , OUT CollisionInfo& outColl) override;
	virtual bool Intersects(Circle* _circle          , OUT CollisionInfo& outColl) override;
	
	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void Render() override;
	virtual void OnUpdateWorldTransform() override;
	
	virtual COLLIDER_TYPE GetColliderType() override;

private:
	void RenderVertexWithoutTransform();

private:
	D3DXVECTOR2 m_center;
	float       m_radius;
	D3DXVECTOR2 m_worldCenter;
	float       m_worldRadius;
	
/* === === === === ===
*   Getter / Setter
* === === === === === */
public:
	float GetRadius() const        { return m_worldRadius; }
	float GetRadiusSq() const      { return m_worldRadius * m_worldRadius; }
	void  SetRadius(float _radius) { m_radius = _radius; }

	D3DXVECTOR2 GetCenter() const                     { return m_worldCenter; }
	void        SetCenter(const D3DXVECTOR2& _center) { m_center = _center; }

	D3DXVECTOR2 GetMin() override { return m_worldCenter - D3DXVECTOR2(m_worldRadius, m_worldRadius); }
	D3DXVECTOR2 GetMax() override { return m_worldCenter + D3DXVECTOR2(m_worldRadius, m_worldRadius); }
};

