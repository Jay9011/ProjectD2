#pragma once
#include "Engine/Object/GameObject.h"

class Scene;

class TestObject : public GameObject
{
public:
	TestObject(Scene* _scene, int _updateOrder = 100, GameObject* _parent = nullptr);
	TestObject(Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
	virtual ~TestObject();

	// GameObject을(를) 통해 상속됨
	virtual void UpdateObject() override;
	virtual void RenderObject() override;
	virtual void PostUpdateObject() override;
	virtual void FinalUpdateObject() override;

	void SetAction(PLAYER_STATE _state);

public:
	Scene* scene;

	bool m_isRight;
	float m_speed;
	PLAYER_STATE m_state;
	Animator* m_animator;
	Collider* m_bodyCollider;
	Collider* m_handCollider;
	Collider* m_sight_u;
	Collider* m_beforePlatform;

	double pressedJumpKey;
	
	Physics m_physics;
#if _DEBUG
	D3DXVECTOR3 m_dir;
#endif

public:
	void ScaleXInverse()
	{
		D3DXVECTOR2 scale = GetScale();
		scale.x *= -1;
		SetScale(scale);
	};

private:
	void SetAnimation();
	void MoveLeftRight();
	void GroundCheck();
};