#pragma once
#include "Game/Objects/Monster/Monster.h"
class MMM : public Monster
{
public:
    MMM(Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
    ~MMM() override;

public:
    // Monster��(��) ���� ��ӵ�
    bool FindOut() override;
    void OnHit(ATK_Info _info) override;
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
*  ���º��� ���� ���
* === === === === === */
private:
    bool  m_isDead = false;
    float m_timer = 0.0f;
    
private:
    void SetAnimation();
    
    void Idle();
    void Patrol();
    void Chase();
    void Attack();
    void AttackEnd();
    void Die();
    void DieEnd();
    
    bool CheckFloor(Collider& _sight);
};

