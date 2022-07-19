#pragma once
class GameObject;
class Component
{
public:
	Component(GameObject* _owner, int updateOrder = 100);
	virtual ~Component();

	virtual void Update() = 0;
	virtual void FinalUpdate() {}
	virtual void Render() {}
	virtual void OnUpdateWorldTransform() {}

private:
	GameObject* m_owner;
	int m_updateOrder;

public:
	GameObject* GetOwner() { return m_owner; }
	int GetUpdateOrder() const { return m_updateOrder; }

};
