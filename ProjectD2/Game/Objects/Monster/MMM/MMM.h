#pragma once
#include "Game/Objects/Monster/Monster.h"
class MMM : public Monster
{
public:
    MMM(Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 110, GameObject* _parent = nullptr);
    ~MMM() override;

public:
    // Monster을(를) 통해 상속됨
    bool FindOut() override;
    bool PlayerLost() override;
    void OnHit(ATK_Info _info, D3DXVECTOR2 _dir) override;
    void StateProcessing() override;
    
    void UpdateAnimation(MON_STATE _state) override;

private:
    Animator* m_animator;
    Collider* m_attackCollider;
    Collider* m_attackRangeCollider;
    Gravity*  m_gravity;

    vector<Collider*> m_sight;
    Collider* m_floorSight;
    
/* === === === === === 
*  상태변경 관련 멤버
* === === === === === */
private:
    bool  m_isDead = false;
    float m_timer = 0.0f;
    
private:
    void SetAnimation();
    
    void Idle();
    void Patrol();
    void Chase();
    void Trace();
    void Attack();
    void AttackEnd();
    void Die();
    void DieEnd();
    
    bool CheckFloor(Collider& _sight);
};

