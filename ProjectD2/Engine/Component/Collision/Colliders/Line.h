#pragma once
#include "Engine/Component/Collision/Collider.h"

class Line : public Collider
{
private:
	Line(const D3DXVECTOR2& _start, const D3DXVECTOR2& _end, class GameObject* _owner, int _updateOrder = 100);
	virtual ~Line() override;
	
public:
	float MinDistSq(const D3DXVECTOR2& _point);
	float MinDistSq(Line* _other);
	D3DXVECTOR2 PointOnLine(float _t);

	// Collider을(를) 통해 상속됨
	virtual bool Contains(const D3DXVECTOR2& _point) override;

	virtual void Update() override;
	virtual void OnUpdateWorldTransform() override;
	virtual void Render() override;

private:
	void RenderVertex();
	void RenderVertexWithoutTransform();

private:
	D3DXVECTOR2 m_start;
	D3DXVECTOR2 m_end;
	D3DXVECTOR2 m_worldStart;
	D3DXVECTOR2 m_worldEnd;
	
public:
	virtual COLLIDER_TYPE GetColliderType() override;
	
	virtual D3DXVECTOR2 GetMin() override;
	virtual D3DXVECTOR2 GetMax() override;

	virtual D3DXVECTOR2 GetStart() { return m_worldStart; }
	virtual D3DXVECTOR2 GetEnd()   { return m_worldEnd; }

	friend class Component;
};

