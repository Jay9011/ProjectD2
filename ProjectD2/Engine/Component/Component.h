#pragma once
class GameObject;
class Component
{
public:
	Component(GameObject* _owner, int _updateOrder = 100);
	virtual ~Component();

	virtual void Update() = 0;
	virtual void FinalUpdate() = 0;
	virtual void OnUpdateWorldTransform() = 0;
	virtual void Render() {}

private:
	GameObject* m_owner;
	int m_updateOrder;

/* === === === === ===
*   Getter / Setter
* === === === === === */
public:
	GameObject* GetOwner() { return m_owner; }
	int GetUpdateOrder() const { return m_updateOrder; }

};
