#pragma once
#include "Engine/Object/GameObject.h"
class Background;
class PlatformRect;
class TestMap_1 : public GameObject
{
public:
	TestMap_1(Scene* _scene, int _updateOrder = 100, GameObject* _parent = nullptr);
	~TestMap_1();

	// 초기 카메라 위치 설정 후 시작 카메라 관련 세팅
	void CameraInit();
    
	// GameObject을(를) 통해 상속됨
	virtual void UpdateObject() override;
	virtual void RenderObject() override;

private:
	Scene* m_scene;

	PlatformRect* m_PlayerStartFlag;
    
	vector<GameObject*> m_platforms;
	vector<Background*> m_backgrounds;
    
	
	PlatformRect* m_Entry;
	

public:
	D3DXVECTOR2 GetPlayerStartPoint();
	void SetMonsters();
};

