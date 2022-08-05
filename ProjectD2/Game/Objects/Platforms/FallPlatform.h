#pragma once
#include "Engine/Object/GameObject.h"

class FallPlatform : public GameObject
{
    enum Anim
    {
        HOLD,
        SINK,
        FIN
    };
public:
    FallPlatform(Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
    ~FallPlatform() override;

    // GameObject을(를) 통해 상속됨
    virtual void UpdateObject() override;
    virtual void RenderObject() override;

private:
    void InitAnim();

private:
    class Animator* m_animator;
    class Collider* m_collider;

    bool  m_isSinked = false;
    float m_timer    = 0.0f;
};

