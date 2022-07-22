#pragma once

class Component
{
protected:
	Component(class GameObject* _owner, int _updateOrder = 100);

public:
	virtual ~Component();
	
public:
	virtual void Update() = 0;
	virtual void FinalUpdate() = 0;
	virtual void OnUpdateWorldTransform() = 0;
	virtual void Render() {}

	/* === === === === ===
	*       Factory
	* === === === === === */
	class Factory
	{
	private:
		Factory() = default;
		~Factory() = default;

	public:
		static class Animator* NewAnimator(class GameObject* _owner, int _updateOrder = 100);
		static class Animator* NewAnimator(class GameObject* _owner, class Shader* _shader, int _updateOrder = 100);
		static class AARect*   NewAARect(const D3DXVECTOR2& _min, const D3DXVECTOR2& _max, class GameObject* _owner, int _updateOrder = 100);
		static class Circle*   NewCircle(const D3DXVECTOR2& _center, float _radius, class GameObject* _owner, int _updateOrder = 100);
		static class Line*     NewLine(const D3DXVECTOR2& _start, const D3DXVECTOR2& _end, class GameObject* _owner, int _updateOrder = 100);

	};

/* === === === === ===
*   Member Variables
* === === === === === */
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

using ADDCOMP = Component::Factory;