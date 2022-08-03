#pragma once
#include "Engine/Object/GameObject.h"
#include "Engine/AI/AI.h"
#include "Engine/Component/PhysicsWorld/Physics.h"
using MonInfo = struct tagMonInfo
{
    float hp;
    float attack;
    float attackRange;
};
class Monster : public GameObject
{
public:
    Monster(Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 110, GameObject* _parent = nullptr);
    virtual ~Monster() override;

public:
    virtual bool FindOut() = 0;
    virtual bool PlayerLost() = 0;
    virtual void OnHit(ATK_Info _info, D3DXVECTOR2 _dir) = 0;
    virtual void StateProcessing() = 0;
    virtual void UpdateAnimation(MON_STATE _state) = 0;

    // GameObject을(를) 통해 상속됨
    void UpdateObject() override;
    void PostUpdateObject() override;
    void FinalUpdateObject() override;
    
protected:
    Scene* m_scene;
    
    MonInfo m_info;
    Physics m_physics;
    AI<Monster, MON_STATE>* m_AI;

    Collider* m_bodyCollider;

    GameObject* m_lastFindPlayer;
    Collider*   m_lastFindPlayerCollider;
    
    /*
    *  상태 관련 변수들
    */
    bool m_isFindPlayer = false;    // 플레이어를 찾은 상태
    
    bool        m_isDamaged = false;    // 피격 상태
    D3DXVECTOR2 m_damagedDir = { 0, 0 };// 피격 방향

/* === === === === === 
*  Getter/Setter
*  === === === === === */
public:
    MonInfo& GetMonInfo() { return m_info; }
    Physics& GetPhysics() { return m_physics; }
    AI<Monster, MON_STATE>* GetAI() { return m_AI; }
    

    void ScaleXInverse()
    {
        D3DXVECTOR2 scale = GetScale();
        scale.x *= -1;
        SetScale(scale);
    };
    
private:
    void SetMonInfo(const MonInfo& _info) { m_info = _info; }
    void SetPhysics(const Physics& _physics) { m_physics = _physics; }
    Physics& GetRefPhysics() { return m_physics; }
    void SetAI(AI<Monster, MON_STATE>* _AI) { m_AI = _AI; }
    
    /*
    * 상태 관련 Getter/Setter
    */
    bool IsDamaged() { return m_isDamaged; }
    
    friend class MonsterFactory;
};

