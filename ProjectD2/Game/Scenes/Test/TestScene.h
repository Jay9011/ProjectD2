#pragma once
class Player;
class TestScene : public Scene
{
public:
	TestScene(Game* _game);
	virtual ~TestScene();

public:
	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Release() override;
	virtual void UpdateScene() override;
	virtual void RenderScene() override;
	
	virtual void SceneEnter() override;
	virtual void SceneExit() override;

private:
	class TestMap_1* testMap_1;
	class Player* testObject;
	class WeaponUI* playerWeapon;
};

