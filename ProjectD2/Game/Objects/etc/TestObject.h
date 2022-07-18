#pragma once
#include "Engine/Object/GameObject.h"
class TestObject : public GameObject
{
public:
	TestObject(Scene* _scene, GameObject* _parent = nullptr);
	TestObject(Scene* _scene, OBJECT_TYPE _type, GameObject* _parent = nullptr);
	virtual ~TestObject();

	// GameObject을(를) 통해 상속됨
	virtual void FinalUpdate() override;
	virtual void FinalRender() override;


private:
	bool m_isRight;
	float m_speed;
	class Texture* m_texture;
	class Shader*  m_shader;

public:
	void ScaleXInverse()
	{
		D3DXVECTOR3 scale = GetScale();
		scale.x *= -1;
		SetScale(scale);
	};
};