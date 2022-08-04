#pragma once
#include "Engine/Object/Transform.h"

enum class UI_STATE
{
	ACTIVE,
	INACTIVE,
    HIDDEN
};
class Component;
class UIObject : public Transform
{
public:
	UIObject(Scene* _scene, int _updateOrder = 100, UIObject* _parent = nullptr);
	virtual ~UIObject() override;

	void Update();
	void UpdateComponent();
	void FinalUpdate();
	void FinalUpdateComponent();
	void Render();
	void RenderComponent();

	void AddComponent(Component* _component);
	void RemoveComponent(Component* _component);

	virtual void UpdateObject() = 0;
	virtual void PostUpdateObject() {};
	virtual void FinalUpdateObject() {};
	virtual void RenderObject() = 0;

private:
	Scene* m_scene;

	UI_STATE m_state;
    
	int  m_updateOrder;
	bool m_isUpdating  = false;
	bool m_isRendering = false;

/* === === === === ===
*     Component
* === === === === === */
private:
	vector<Component*> m_componentList;
    
/* === === === === ===
*  Getter / Setter
* === === === === === */
public:
    const UIObject* GetParent() const override { return (UIObject*)m_parent; }
	Scene* GetScene() const { return m_scene; }
	int GetOrder() const { return m_updateOrder; }

	UI_STATE GetState() const          { return m_state; }
	void     SetState(UI_STATE _state) { m_state = _state; }
    
	bool     IsInactive() const        { return m_state == UI_STATE::INACTIVE; }
    bool     IsActive() const          { return m_state == UI_STATE::ACTIVE; }
    bool     IsHidden() const          { return m_state == UI_STATE::HIDDEN; }
};

