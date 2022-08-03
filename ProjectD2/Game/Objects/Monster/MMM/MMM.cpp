#include "Framework.h"
#include "MMM.h"

#include "Engine/Component/Component.h"
#include "Engine/Resource/Shader.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Component/Collision/Collision.h"
#include "Engine/Component/PhysicsWorld/Gravity/Gravity.h"

#include "Game/AI/State/CommonMonState.h"
#include "Game/Objects/Charactor/Player.h"

MMM::MMM(Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
    Monster(_scene, _type, _updateOrder, _parent)
{
    /*
    * Animator
    */
    m_animator = ADDCOMP::NewAnimator(this, SHADER(L"AlphaShader"));

    SetAnimation();
    
    /*
    * Collider
    */
    m_bodyCollider = ADDCOMP::NewAARect({ -20, -12 }, { 20, 30 }, this);
    m_bodyCollider->IsActive(true);
    m_attackCollider = ADDCOMP::NewAARect({-25, -12}, {0, 30}, OBJECT_TYPE::MONSTER_ATK, this);
    m_attackCollider->IsActive(false);
    m_attackCollider->SetCallbackOnCollisionEnter([this](Collider* _other) {
        std::cout << "MMM Attack Hit!" << std::endl;
        if (_other->GetOwner()->GetType() == OBJECT_TYPE::PLAYER)
        {
            Player* player = dynamic_cast<Player*>(_other->GetOwner());
            if(player == nullptr)
                return;
            
            player->Damage(m_info.attack);
        }
        });
    m_attackRangeCollider = ADDCOMP::NewAARect({-25, -12}, {0, 30}, OBJECT_TYPE::MONSTER_SIGHT, this);
    m_attackRangeCollider->IsActive(true);
    m_floorSight = ADDCOMP::NewLine({ -20, 0 }, { -20, 100 }, OBJECT_TYPE::MONSTER_SIGHT, this);
    m_floorSight->IsActive(true);
    m_sight.emplace_back(ADDCOMP::NewLine({0, 0}, {-150, -30}, OBJECT_TYPE::MONSTER_SIGHT, this));
    m_sight.emplace_back(ADDCOMP::NewLine({0, 0}, {-150, 0}, OBJECT_TYPE::MONSTER_SIGHT, this));
    m_sight.emplace_back(ADDCOMP::NewLine({0, 0}, {-150, 30}, OBJECT_TYPE::MONSTER_SIGHT, this));
    
    for (auto& sight : m_sight)
    {
        sight->IsActive(true);
    }
    
    /*
    * Physics
    */
    m_gravity = ADDCOMP::NewGravity(m_physics, this);
    /*
    * AI Setting
    */
    m_AI = new AI<Monster, MON_STATE>(this);
    m_AI->AddState(new CommonMonState::Idle());
    m_AI->AddState(new CommonMonState::Patrol());
    m_AI->AddState(new CommonMonState::Chase());
    m_AI->AddState(new CommonMonState::Trace());
    m_AI->AddState(new CommonMonState::Attack());
    m_AI->AddState(new CommonMonState::Die());
    m_AI->AddState(new CommonMonState::Global());

    m_AI->ChangeState(MON_STATE::IDLE);
    m_AI->SetGlobalState(MON_STATE::GLOBAL);
}

MMM::~MMM() = default;

bool MMM::FindOut()
{
    vector<std::pair<Collider*, Collider*>> collided;
    
    for (auto& sight : m_sight)
    {
        m_scene->GetCollisionMgr()->CheckCollision(sight, OBJECT_TYPE::PLAYER, collided);
    }

    if (!collided.empty())
    {
        m_lastFindPlayerCollider = collided.back().second;
        m_lastFindPlayer = collided.back().second->GetOwner();
        m_isFindPlayer = true;
    }
    
    return m_isFindPlayer;
}

bool MMM::PlayerLost()
{
    bool result = false;
    
    if (m_lastFindPlayer == nullptr)
    {
        return result;
    }
    else if (m_lastFindPlayer->GetState() == OBJECT_STATE::DEAD) // 플레이어가 죽은 상태라면
    {
        
    }
    
    return result;
}

void MMM::OnHit(ATK_Info _info, D3DXVECTOR2 _dir)
{
    m_isDamaged = true;
    m_damagedDir = _dir;

    ATK_Info info = _info;
    m_info.hp -= info.damage;

    // 플레이어를 발견하지 못한 상태에서 공격에 맞은 경우 플레이어를 찾기 시작한다.
    if (!m_isFindPlayer)
    {
        m_AI->ChangeState(MON_STATE::TRACE);
    }
}

void MMM::StateProcessing()
{
    MON_STATE state = m_AI->GetState();

    switch (state)
    {
    case MON_STATE::IDLE:
        Idle();
        break;
    case MON_STATE::PATROL:
        Patrol();
        break;
    case MON_STATE::TRACE:
        Trace();
        break;
    case MON_STATE::CHASE:
        Chase();
        break;
    case MON_STATE::ATTACK:
        Attack();
        break;
    case MON_STATE::DIE:
        if(!m_isDead)
            Die();
        break;
    default:
        break;
    }
}

void MMM::UpdateAnimation(MON_STATE _state)
{
    switch (_state)
    {
    case MON_STATE::IDLE:
        m_animator->Find((int)MON_STATE::IDLE)->Play();
        break;
    case MON_STATE::PATROL:
        m_animator->Find((int)MON_STATE::PATROL)->Play();
        break;
    case MON_STATE::TRACE:
        m_animator->Find((int)MON_STATE::TRACE)->Play();
        break;
    case MON_STATE::CHASE:
        m_animator->Find((int)MON_STATE::CHASE)->Play();
        break;
    case MON_STATE::ATTACK:
        m_animator->Find((int)MON_STATE::ATTACK)->Play();
        break;
    case MON_STATE::DIE:
        m_animator->Find((int)MON_STATE::DIE)->Play();
        break;
    case MON_STATE::GLOBAL:
        break;
    default:
        break;
    }
}

void MMM::SetAnimation()
{
    for (int i = 0; i < (int)MON_STATE::FIN; i++)
    {
        switch ((MON_STATE)i)
        {
        case MON_STATE::IDLE:
            m_animator->LoadXML("Monster\\", "mmm_Idle", ANIM_PLAY_TYPE::PINGPONG, 0.1f);
            break;
        case MON_STATE::PATROL:
            m_animator->LoadXML("Monster\\", "mmm_Move", ANIM_PLAY_TYPE::LOOP, 0.15f);
            break;
        case MON_STATE::TRACE:
            m_animator->LoadXML("Monster\\", "mmm_Move", ANIM_PLAY_TYPE::LOOP, 0.1f);
            break;
        case MON_STATE::CHASE:
            m_animator->LoadXML("Monster\\", "mmm_Move", ANIM_PLAY_TYPE::LOOP, 0.1f);
            break;
        case MON_STATE::ATTACK:
            m_animator->LoadXML("Monster\\", "mmm_ATK", ANIM_PLAY_TYPE::LOOP, 0.1f);
            m_animator->SetEndEvent(i, [this]() {
                AttackEnd();
                });
            break;
        case MON_STATE::DIE:
            m_animator->LoadXML("SFX\\Explosion\\", "Explosion01", ANIM_PLAY_TYPE::ONCE, 0.05f);
            m_animator->SetEndEvent(i, [this]() {
                DieEnd();
                });
            break;
        case MON_STATE::GLOBAL:
            break;
        default:
            // Animation 등록 갯수가 맞지 않으면 에러
            assert(false);
            break;
        }
    }
}

void MMM::Idle()
{
    if (m_timer <= 2.0f)
    {
        m_timer += fDT;
        return;
    }
    
    m_timer = 0.0f;
    m_AI->ChangeState(MON_STATE::PATROL);
    return;
}

void MMM::Patrol()
{
    if (m_timer <= 1.0f && CheckFloor(*m_sight[2]))    // 순찰중 & 바닥이 없으면 순찰 종료
    {
        m_timer += fDT;
            
        if (GetScale().x < 0)
        {
            // 오른쪽을 보고 있을 때,
            m_physics.MovingX(V_RIGHT.x * 0.5f);
        }
        else
        {
            // 왼쪽을 보고 있을 때,
            m_physics.MovingX(V_LEFT.x * 0.5f);
        }
        
        return;
    }
    
    // 순찰 종료
    m_timer = 0.0f;
    m_AI->ChangeState(MON_STATE::IDLE);
    return;
}

void MMM::Trace()
{
    if (m_timer <= 10.0f && CheckFloor(*m_sight[2]))    // 조사중 바닥이 없으면 조사 종료
    {
        m_timer += fDT;

        if (m_damagedDir.x < 0) // 왼쪽으로 조사할 때
        {
            if (GetScale().x < 0)   // 오른쪽을 보고 있다면
                ScaleXInverse();
                
            m_physics.MovingX(V_LEFT.x * 0.5f);
        }
        else
        {
            if (GetScale().x > 0)   // 왼쪽을 보고 있다면
                ScaleXInverse();
            
            m_physics.MovingX(V_RIGHT.x * 0.5f);
        }

        return;
    }

    // 추적 종료
    m_timer = 0.0f;
    m_AI->ChangeState(MON_STATE::IDLE);
    return;
}

void MMM::Chase()
{
    GameObject* player = m_lastFindPlayer;

    if (player != nullptr && player->GetState() != OBJECT_STATE::DEAD)
    {
        if (Collision(m_attackRangeCollider, m_lastFindPlayerCollider)) // 공격 범위에 들어오면 공격을 시도한다.
        {
            m_AI->ChangeState(MON_STATE::ATTACK);
            return;
        }
        
        if (CheckFloor(*m_floorSight))    // 바닥이 있을 때 Player를 쫓아간다.
        {
            D3DXVECTOR2 dir = player->GetWorldPos() - GetWorldPos();
            if (dir.x < 0)
            {
                if (GetScale().x < 0)
                    ScaleXInverse();
                
                m_physics.MovingX(V_LEFT.x);
            }
            else
            {
                if (GetScale().x > 0)
                    ScaleXInverse();
                
                m_physics.MovingX(V_RIGHT.x);
            }
        }
    }
}

void MMM::Attack()
{
    m_attackCollider->IsActive(true);
}

void MMM::AttackEnd()
{
    m_attackCollider->IsActive(false);
    m_AI->RevertToPreviousState();
    return;
}

void MMM::Die()
{
    m_isDead = true;
    m_bodyCollider->IsActive(false);
    m_attackCollider->IsActive(false);
    m_physics.force = { 0, 0 };
    m_gravity->Delete();
}

void MMM::DieEnd()
{
    SetState(OBJECT_STATE::DEAD);
}

bool MMM::CheckFloor(Collider& _sight)
{
    bool result = false;
    
    vector<std::pair<Collider*, Collider*>> collided;
    // 바닥을 보는 sight는 2번 sight
    m_scene->GetCollisionMgr()->CheckCollision(&_sight, OBJECT_TYPE::PLATFORM, collided);

    if (!collided.empty())
    {
        result = true;
    }
    
    return result;
}
