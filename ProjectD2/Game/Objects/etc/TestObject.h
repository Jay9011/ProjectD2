#pragma once
#include "Engine/Object/GameObject.h"

class TestObject : public GameObject
{
public:
	TestObject(Scene* _scene, GameObject* _parent = nullptr);
	TestObject(Scene* _scene, OBJECT_TYPE _type, GameObject* _parent = nullptr);
	virtual ~TestObject();

	// GameObject을(를) 통해 상속됨
	virtual void UpdateObject() override;
	virtual void RenderObject() override;

	void SetAction(PLAYER_STATE _state);

	bool m_isRight;
	float m_speed;
	PLAYER_STATE m_state;
	class Animator* m_animator;
	class Collider* m_bodyCollider;

public:
	void ScaleXInverse()
	{
		D3DXVECTOR3 scale = GetScale();
		scale.x *= -1;
		SetScale(scale);
	};

private:
	void SetAnimation();
};