#pragma once
#include "Game/Objects/Monster/Monster.h"

class MMM : public Monster
{
public:
    MMM(Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
    ~MMM() override;

public:
    // Monster을(를) 통해 상속됨
    bool FindOut() override;
    void StateProcessing() override;
    
    void UpdateAnimation(MON_STATE _state) override;

private:
    Animator* m_animator;
    Collider* m_attackCollider;

    vector<Collider*> m_sight;
    
private:
    void SetAnimation();
    
};

