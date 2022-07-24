#pragma once
#include "Engine/Component/Component.h"

struct CollisionInfo
{
	D3DXVECTOR2 point;
	D3DXVECTOR2 vector;
	class Collider* other;
};

class Collider : public Component
{
protected:
	Collider(GameObject* _owner, int _updateOrder = 100);
	Collider(const Collider& _other) = delete;
	virtual ~Collider() override;

public:
	virtual COLLIDER_TYPE GetColliderType() = 0;

	virtual bool Contains(const D3DXVECTOR2& _point) = 0;

	virtual void Render() {};

	virtual void OnCollisionEnter(Collider* _other) { m_state = COLLISION_STATE::ENTER; };
	virtual void OnCollisionStay(Collider* _other)  { m_state = COLLISION_STATE::STAY; };
	virtual void OnCollisionExit(Collider* _other)  { m_state = COLLISION_STATE::EXIT; };

	// Component을(를) 통해 상속됨
	virtual void Update() override = 0;
	void FinalUpdate() final;
	virtual void OnUpdateWorldTransform() override = 0;

private:
	bool isActive;
	
	UINT m_id;
	static UINT m_idCounter;

	COLLISION_STATE m_state;

/* === === === === ===
*   Getter / Setter
* === === === === === */
public:
	bool IsActive() { return isActive; }
	bool IsActive(const bool& _b) { isActive = _b; return isActive; }

	bool IsCollided(const bool& _b) { isCollided = _b; return isCollided; }

	virtual D3DXVECTOR2 GetMin() = 0;
	virtual D3DXVECTOR2 GetMax() = 0;

	UINT GetID() { return m_id; }

	/* 디버그용 */
protected:
	bool isCollided   = false;
	D3DCOLOR offColor = 0xFF33FF33;
	D3DCOLOR onColor  = 0xFFFF3333;

	Collider& operator = (const Collider& _other) = delete;
};

