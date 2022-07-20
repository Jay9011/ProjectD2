#pragma once
#include "Engine/Component/Collision/Collider.h"

class AARect : public Collider
{
public:
	AARect(const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, class GameObject* _owner, int _updateOrder = 100);
	virtual ~AARect() override;

public:
	// Collider을(를) 통해 상속됨
	virtual bool Contains(const D3DXVECTOR2& _point) override;

	virtual bool Intersects(const D3DXVECTOR2& _point) override;
	virtual bool Intersects(AARect& _rect, OUT CollisionInfo& outColl) override;
	
	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void OnUpdateWorldTransform() override;
	virtual void Render() override;

public:
	virtual COLLIDER_TYPE GetColliderType() override;

private:
	void RenderVertexWithoutTransform();

private:
	D3DXVECTOR2 m_min;
	D3DXVECTOR2 m_max;
	D3DXVECTOR2 m_worldMin;
	D3DXVECTOR2 m_worldMax;
};

