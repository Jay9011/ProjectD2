#pragma once
class TestObject;
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

	class TestObject* testObject;

	class TestObject2* testObject2;

	class TestObjectCircle* testObjectCircle;
	class TestObjectLine* testline;

	class TestMap_1* testMap_1;

};

