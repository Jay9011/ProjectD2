#pragma once
class ToolScene : public Scene
{
public:
	ToolScene(Game* _game);
	virtual ~ToolScene();

public:
	// Scene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Release() override;
	virtual void UpdateScene() override;
	virtual void RenderScene() override;

private:
	

};

