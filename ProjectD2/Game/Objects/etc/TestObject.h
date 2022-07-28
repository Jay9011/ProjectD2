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

private:
	void Move();
	void ChangeWeapon();
	void StateProcessing();
	void AnimationProcessing();

	void UpdateState(PLAYER_STATE _state, PLAYER_EQUIP_TYPE _equip);
	void UpdateAnimation();
	void SetAnimation();
	void BodyPlatformCheck();
	void HandPlatformCheck();
    
public:
	Scene* scene;

	bool m_isRight;
    
	PLAYER_STATE m_prevState;
	PLAYER_STATE m_state;
	PLAYER_EQUIP_TYPE m_prevEquip;
	PLAYER_EQUIP_TYPE m_equip;

	Physics m_physics;

	Animator* m_animator;
	Collider* m_bodyCollider;
	Collider* m_handCollider;
	Collider* m_sight_u;
	Collider* m_beforePlatform;
    
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

};