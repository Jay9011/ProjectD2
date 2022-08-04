#pragma once
#include "GameObject.h"

class Component;
class UIObject : public GameObject
{
public:
	UIObject(Scene* _scene, int _updateOrder = 100, GameObject* _parent = nullptr);
	virtual ~UIObject() override;
    
	virtual void UpdateObject() = 0;
	virtual void PostUpdateObject() {};
	virtual void FinalUpdateObject() {};
	virtual void RenderObject() = 0;

private:
	Scene* m_scene;

/* === === === === ===
*  Getter / Setter
* === === === === === */
public:
	Scene* GetScene() const { return m_scene; }
};

