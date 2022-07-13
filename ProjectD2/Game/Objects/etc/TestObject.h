#pragma once
#include "Engine/Object/GameObject.h"
class TestObject : public GameObject
{
public:
	TestObject(Game* _game, Scene* _scene, GameObject* _parent = nullptr);
	TestObject(Game* _game, Scene* _scene, OBJECT_TYPE _type, GameObject* _parent = nullptr);
	virtual ~TestObject();

	// GameObject을(를) 통해 상속됨
	virtual void UpdateObject() override;
	virtual void Render() override;


private:
	bool m_isRight;
	float m_speed;
	Texture* m_texture;

public:
	void ScaleXInverse()
	{
		D3DXVECTOR3 scale = GetScale();
		scale.x *= -1;
		SetScale(scale);
	};
};