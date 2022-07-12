#pragma once
class TestObject;
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

private:
	VERTEXCOLOR vertices[6];
	
	TestObject* m_gameObj;

};

