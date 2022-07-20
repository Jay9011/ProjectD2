#pragma once
#include "Engine/Component/Component.h"

class AARect;
struct CollisionInfo
{
	D3DXVECTOR2 point;
	D3DXVECTOR2 normal;
	class Collider* other;
};

class Collider : public Component
{
protected:
	Collider(GameObject* _owner, int _updateOrder = 100);
	virtual ~Collider() override;

public:
	virtual COLLIDER_TYPE GetColliderType() = 0;

	virtual bool Contains(const D3DXVECTOR2& _point) = 0;

			bool Intersects(Collider* _other);
	virtual bool Intersects(const D3DXVECTOR2& _point) = 0;
	virtual bool Intersects(AARect& _rect, OUT CollisionInfo& outColl) = 0;
	//virtual bool Intersects(const AARect& _rect, OUT float& outT, OUT CollisionInfo& outColl) = 0;
	

	virtual void OnCollisionEnter(Collider* _other) {}
	virtual void OnCollisionStay (Collider* _other) {}
	virtual void OnCollisionExit (Collider* _other) {}

	bool& IsActive() { return isActive; }
	bool& IsActive(const bool& _b) { isActive = _b; return isActive; }

	// Component을(를) 통해 상속됨
	virtual void Update() override = 0;
	virtual void FinalUpdate() override = 0;
	virtual void OnUpdateWorldTransform() override = 0;

private:
	bool isActive;

	/* 디버그용 */
protected:
	bool isCollided   = false;
	D3DCOLOR offColor = 0xFF33FF33;
	D3DCOLOR onColor  = 0xFFFF3333;
};

