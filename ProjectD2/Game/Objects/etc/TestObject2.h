#pragma once
#include "Engine/Object/GameObject.h"
class Animator;
class TestObject2 : public GameObject
{
public:
	TestObject2(Scene* _scene, int _updateOrder = 100, GameObject* _parent = nullptr);
	TestObject2(Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
	virtual ~TestObject2();

	// GameObject을(를) 통해 상속됨
	virtual void UpdateObject() override;
	virtual void RenderObject() override;

	bool m_isRight;
	float m_speed;
	float m_width;
	float m_halfWidth;
	float m_height;
	float m_halfHeight;

	class AARect* m_bodyCollider;

	Animator* m_animator;


	void SetAnimation();
	
	void ScaleXInverse()
	{
		D3DXVECTOR2 scale = GetScale();
		scale.x *= -1;
		SetScale(scale);
	};
};

