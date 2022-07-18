#pragma once
#include "Engine/Object/GameObject.h"
class TestObject2 : public GameObject
{
public:
	TestObject2(Scene* _scene, GameObject* _parent = nullptr);
	TestObject2(Scene* _scene, OBJECT_TYPE _type, GameObject* _parent = nullptr);
	virtual ~TestObject2();

	// GameObject을(를) 통해 상속됨
	virtual void UpdateObject() override;
	virtual void Render() override;


private:
	bool m_isRight;
	float m_speed;

public:
	void ScaleXInverse()
	{
		D3DXVECTOR3 scale = GetScale();
		scale.x *= -1;
		SetScale(scale);
	};
};

