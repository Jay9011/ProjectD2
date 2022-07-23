#pragma once
#include "Engine/Object/GameObject.h"

class TestMap_1 : public GameObject
{
public:
	TestMap_1(Scene* _scene, GameObject* _parent = nullptr);
	~TestMap_1();


	// GameObject��(��) ���� ��ӵ�
	virtual void UpdateObject() override;
	virtual void RenderObject() override;

	vector<Collider*> m_platforms;
	Texture* m_texture;
};

