#pragma once
class TestScene : public Scene
{
public:
	TestScene(Game* _game);
	virtual ~TestScene();

public:
	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Release() override;

	virtual void Update() override;
	virtual void Render() override;

private:
	VERTEXCOLOR vertices[6];
	
};

