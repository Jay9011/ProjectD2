#pragma once
#include "Engine/Object/GameObject.h"
class Background;
class PlatformRect;
class TestMap_1 : public GameObject
{
public:
	TestMap_1(Scene* _scene, int _updateOrder = 100, GameObject* _parent = nullptr);
	~TestMap_1();

	// �ʱ� ī�޶� ��ġ ���� �� ���� ī�޶� ���� ����
	void CameraInit();
    
	// GameObject��(��) ���� ��ӵ�
	virtual void UpdateObject() override;
	virtual void RenderObject() override;

private:
	vector<Background*> m_backgrounds;
    
	vector<GameObject*> m_platforms;
	PlatformRect* m_Entry;
	
	PlatformRect* m_PlayerStartFlag;

public:
	D3DXVECTOR2 GetPlayerStartPoint();
};

