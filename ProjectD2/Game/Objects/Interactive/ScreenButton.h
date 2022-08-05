#pragma once
#include "Engine/Object/GameObject.h"
class Door;
class ScreenButton : public GameObject
{
    enum Anim
    {
        OFF,
        ON,
        FIN
    };
public:
    ScreenButton(Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
    ~ScreenButton() override;

    // GameObject을(를) 통해 상속됨
    virtual void UpdateObject() override;
    virtual void PostUpdateObject() override;
    virtual void FinalUpdateObject() override;
    virtual void RenderObject() override;

private:
    void InitAnimation();
    
private:
    class Texture*  m_keyImg;
    class Shader*   m_shader;
    
    class Animator* m_animator;
    class Collider* m_InteractBox;

    bool m_inRange = false;
    bool m_isUsed  = false;
    
    class Door* m_interactObject;

/* === === === === === 
* Getter / Setter
* === === === === === */
public:
    void SetInteractObject(Door* _object) { m_interactObject = _object; }

    bool GetIsUsed() { return m_isUsed; }

    Collider* GetInteractBox() { return m_InteractBox; }
};

