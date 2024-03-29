#pragma once
#include "Engine/Object/GameObject.h"
class Background;
class PlatformRect;
class Player;
class DialogUI;
class DialogEvent;
class Door;
class ScreenButton;
class Effect;
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

	// Dialog
	DialogUI* m_dialogUI;

	// Player
	Player* m_player;
	PlatformRect* m_PlayerStartFlag;
    
    // Maps
	vector<GameObject*> m_platforms;
	vector<Background*> m_backgrounds;
    
	// Events
	float m_eventTimer;
	DialogEvent* m_enterEvent;
    
	DialogEvent*  m_doorOpenEvent;	// 문 열기 이벤트 관련
	ScreenButton* button_01;
	Door*         door_01;
    
	DialogEvent* monsterFindEvent;	// 몬스터 발견 이벤트 관련
    Monster*     monster_01;
    
	DialogEvent* monsterRegion;		// 몬스터 소환 이벤트 관련
	GameObject*  monsterRegionObject;
	bool         monsterExist = true;
	float        monsterTimer = 0.0f;

	DialogEvent* factoryFindEvent;	// 공장 발견 이벤트
    
	DialogEvent* finishEvent;		// 마지막 이벤트

public:
	D3DXVECTOR2 GetPlayerStartPoint();
	void SetMonsters();
    
    void SetPlayer(Player* _player) { m_player = _player; }
    void SetDialogUI(DialogUI* _ui) { m_dialogUI = _ui; }
};

