#pragma once
class ToolScene : public Scene
{
public:
	ToolScene(Game* _game);
	virtual ~ToolScene();

public:
	// Scene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Release() override;
	virtual void UpdateScene() override;
	virtual void RenderScene() override;

private:
	class TestObject* testObject;
	D3DXVECTOR3 testObjectPos;
	D3DXVECTOR3 testObjectSize;
	float       testObjectAngle;

	class TestObject* testObject2;
};

