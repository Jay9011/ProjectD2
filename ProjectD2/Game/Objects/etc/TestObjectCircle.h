#pragma once
#include "Engine/Object/GameObject.h"
class TestObjectCircle : public GameObject
{
public:
	TestObjectCircle(Scene* _scene, GameObject* _parent = nullptr);
	TestObjectCircle(Scene* _scene, OBJECT_TYPE _type, GameObject* _parent = nullptr);
	virtual ~TestObjectCircle();

	// GameObject��(��) ���� ��ӵ�
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

