#pragma once

class TestObjectLine : public GameObject
{
public:
	TestObjectLine(Scene* _scene, GameObject* _parent = nullptr);
	TestObjectLine(Scene* _scene, OBJECT_TYPE _type, GameObject* _parent = nullptr);
	virtual ~TestObjectLine();

	// GameObject��(��) ���� ��ӵ�
	virtual void UpdateObject() override;
	virtual void RenderObject() override;
	
	class Line* m_line;
};

