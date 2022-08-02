#pragma once
#include "Engine/Component/Component.h"

enum class COLLIDER_TYPE
{
	LINE,
	AARECT,
	CIRCLE
};

enum class COLLISION_STATE
{
    NONE,
	ENTER,
	STAY,
	EXIT
};

struct CollisionInfo
{
	D3DXVECTOR2 point;
	D3DXVECTOR2 vector;
	class Collider* other;
};

struct CollisionCheck
{
	bool isCollision;
	bool isThisFrmChk;
	class Collider* other;

	CollisionCheck() { isCollision = false; isThisFrmChk = false; }
	CollisionCheck(bool _b1, bool _b2, Collider* _other) { isCollision = _b1; isThisFrmChk = _b2; other = _other; }
};

class Collider : public Component
{
protected:
	Collider(GameObject* _owner, int _updateOrder = 100, string _tag = "");
	Collider(OBJECT_TYPE _type, GameObject* _owner, int _updateOrder = 100, string _tag = "");
	Collider(const Collider& _other) = delete;
	virtual ~Collider() override;

public:
	virtual COLLIDER_TYPE GetColliderType() = 0;

	virtual bool Contains(const D3DXVECTOR2& _point) = 0;

	virtual void Render() {};

	void OnCollision(Collider* _other);
	void OnCollisionEnter(Collider* _other) { m_state = COLLISION_STATE::ENTER; if (m_callbackOnCollisionEnter) m_callbackOnCollisionEnter(_other); };
	void OnCollisionStay(Collider* _other)  { m_state = COLLISION_STATE::STAY;  if (m_callbackOnCollisionStay) m_callbackOnCollisionStay(_other); };
	void OnCollisionExit(Collider* _other)  { m_state = COLLISION_STATE::EXIT;  if (m_callbackOnCollisionExit) m_callbackOnCollisionExit(_other); };

	void SetCallbackOnCollisionEnter(std::function<void(OUT Collider*)> const& Event) { m_callbackOnCollisionEnter = Event; };
	void SetCallbackOnCollisionStay(std::function<void(OUT Collider*)> const& Event) { m_callbackOnCollisionStay = Event; };
	void SetCallbackOnCollisionExit(std::function<void(OUT Collider*)> const& Event) { m_callbackOnCollisionExit = Event; };

	// Component을(를) 통해 상속됨
	virtual void Update() override = 0;
	void FinalUpdate() final;
	virtual void OnUpdateWorldTransform() override = 0;

private:
	void ActiveOff();

public:
	ColliderOptions options;

private:
	GameObject* m_owner;
	OBJECT_TYPE m_type;
	bool isActive;
	
	UINT m_id;
	static UINT m_idCounter;
	string m_tag;

	COLLISION_STATE m_state;

	map<UINT, CollisionCheck> m_collisionMap;

	std::function<void(OUT Collider*)> m_callbackOnCollisionEnter;
	std::function<void(OUT Collider*)> m_callbackOnCollisionStay;
	std::function<void(OUT Collider*)> m_callbackOnCollisionExit;

/* === === === === ===
*   Getter / Setter
* === === === === === */
public:
	bool IsActive() { return isActive; }
	bool IsActive(const bool& _b) { isActive = _b; if (!_b) { ActiveOff(); } return isActive; }

	bool IsCollided(const bool& _b) { isCollided = _b; return isCollided; }

	virtual D3DXVECTOR2 GetMin() = 0;
	virtual D3DXVECTOR2 GetMax() = 0;
	FRECT GetRect() { return { GetMin().x, GetMin().y, GetMax().x, GetMax().y }; }

	UINT GetID() { return m_id; }
    
    string GetTag()            { return m_tag; }
    void   SetTag(string _tag) { m_tag = _tag; }

    COLLISION_STATE GetState() { return m_state; }

	/* 디버그용 */
protected:
	bool isCollided   = false;
	D3DCOLOR offColor = 0xFF33FF33;
	D3DCOLOR onColor  = 0xFFFF3333;

	Collider& operator = (const Collider& _other) = delete;
};

