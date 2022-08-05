#pragma once
class Player;
class TestScene : public Scene
{
public:
	TestScene(Game* _game);
	virtual ~TestScene();

public:
	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Release() override;
	virtual void UpdateScene() override;
	virtual void RenderScene() override;
	
	virtual void SceneEnter() override;
	virtual void SceneExit() override;

private:
	// Map
	class TestMap_1* testMap_1;
    // Player ����
	class Player*   player;
	class WeaponUI* playerWeaponUI;
	class HeartUI*  playerHeartUI;
	// Dialog
    class DialogUI* dialogUI;
};

