#include "Framework.h"
#include "MMM.h"

#include "Engine/Component/Component.h"
#include "Engine/Resource/Shader.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Component/Collision/Collision.h"

#include "Game/AI/State/CommonMonState.h"

MMM::MMM(Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
    Monster(_scene, _type, _updateOrder, _parent)
{

    /*
    * Animator
    */
    m_animator = ADDCOMP::NewAnimator(this, SHADER(L"AlphaShader"));

    SetAnimation();
    
    /*
    * Physics
    */
    ADDCOMP::NewGravity(m_physics, this);
    
    /*
    * Collider
    */
    m_bodyCollider = ADDCOMP::NewAARect({ -20, -12 }, { 20, 30 }, this);
    m_bodyCollider->IsActive(true);
    m_sight.emplace_back(ADDCOMP::NewLine({0, 0}, {-150, 0}, this));
    
    for (auto& sight : m_sight)
    {
        sight->IsActive(true);
    }

    /*
    * AI Setting
    */
    m_AI = new AI<Monster, MON_STATE>(this);
    m_AI->AddState(new CommonMonState::Idle());
    m_AI->AddState(new CommonMonState::Patrol());
    m_AI->AddState(new CommonMonState::Chase());
    m_AI->AddState(new CommonMonState::Attack());
    m_AI->AddState(new CommonMonState::Die());
    m_AI->AddState(new CommonMonState::Global());

    m_AI->ChangeState(MON_STATE::IDLE);
    m_AI->SetGlobalState(MON_STATE::GLOBAL);
}

MMM::~MMM() = default;

bool MMM::FindOut()
{
    return false;
}

void MMM::StateProcessing()
{
    MON_STATE state = m_AI->GetState();

    switch (state)
    {
    case MON_STATE::IDLE:
        break;
    case MON_STATE::PATROL:
        break;
    case MON_STATE::CHASE:
        break;
    case MON_STATE::ATTACK:
        break;
    case MON_STATE::DIE:
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
        case MON_STATE::CHASE:
            m_animator->LoadXML("Monster\\", "mmm_Move", ANIM_PLAY_TYPE::LOOP, 0.1f);
            break;
        case MON_STATE::ATTACK:
            m_animator->LoadXML("Monster\\", "mmm_ATK", ANIM_PLAY_TYPE::ONCE, 0.1f);
            break;
        case MON_STATE::DIE:
            m_animator->LoadXML("SFX\\Explosion\\", "Explosion01", ANIM_PLAY_TYPE::ONCE, 0.05f);
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
