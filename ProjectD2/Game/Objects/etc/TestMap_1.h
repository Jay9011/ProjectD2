#pragma once
#include "Engine/Object/GameObject.h"
class PlatformRect;
class TestMap_1 : public GameObject
{
public:
	TestMap_1(Scene* _scene, int _updateOrder = 100, GameObject* _parent = nullptr);
	~TestMap_1();


	// GameObject��(��) ���� ��ӵ�
	virtual void UpdateObject() override;
	virtual void RenderObject() override;

	vector<GameObject*> m_platforms;
	PlatformRect* m_Entry;
};

