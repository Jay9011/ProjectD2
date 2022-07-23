#pragma once
#include "Engine/Object/GameObject.h"
class TestObjectCircle : public GameObject
{
public:
	TestObjectCircle(Scene* _scene, int _updateOrder = 100, GameObject* _parent = nullptr);
	TestObjectCircle(Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
	virtual ~TestObjectCircle();

	// GameObject을(를) 통해 상속됨
	virtual void UpdateObject() override;
	virtual void RenderObject() override;

	bool m_isRight;
	float m_speed;

	class Circle* m_bodyCollider;

	void ScaleXInverse()
	{
		D3DXVECTOR3 scale = GetScale();
		scale.x *= -1;
		SetScale(scale);
	};
};

