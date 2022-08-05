#pragma once
#include "Engine/Object/GameObject.h"

class Door : public GameObject
{
    enum Anim
    {
        CLOSE,
        OPEN,
        FIN
    };
public:
    Door(Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
    ~Door() override;

    void Interact();

    // GameObject을(를) 통해 상속됨
    virtual void UpdateObject() override;
    virtual void RenderObject() override;
    
private:
    void InitAnimation();

private:
    class Animator* m_animator;
    class Collider* m_collider;

    bool m_inRange = false;
    bool m_isUsed = false;

    std::function<void()> m_openFunc;

public:
    void SetOpenFunc(std::function<void()> _func) { m_openFunc = _func; }
    bool IsUsed() { return m_isUsed; }
};

