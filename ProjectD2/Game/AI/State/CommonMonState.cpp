#include "Framework.h"
#include "CommonMonState.h"

#include "Game/Objects/Monster/Monster.h"

/* === === === === === 
*  IDLE 상태
*  === === === === === */
void CommonMonState::Idle::Enter(Monster* _entity)
{
    _entity->UpdateAnimation(MON_STATE::IDLE);
}

void CommonMonState::Idle::Update(Monster* _entity)
{
    if (_entity->FindOut())
    {
        _entity->GetAI()->ChangeState(MON_STATE::CHASE);
    }
}

void CommonMonState::Idle::Exit(Monster* _entity)
{
}

/* === === === === ===
*  Patrol 상태
*  === === === === === */
void CommonMonState::Patrol::Enter(Monster* _entity)
{
    _entity->ScaleXInverse();
    _entity->UpdateAnimation(MON_STATE::PATROL);
}

void CommonMonState::Patrol::Update(Monster* _entity)
{
    if (_entity->FindOut())
    {
        _entity->GetAI()->ChangeState(MON_STATE::CHASE);
    }
}

void CommonMonState::Patrol::Exit(Monster* _entity)
{
}

/* === === === === ===
*  Trace 상태
*  === === === === === */
void CommonMonState::Trace::Enter(Monster* _entity)
{
    _entity->UpdateAnimation(MON_STATE::TRACE);
}

void CommonMonState::Trace::Update(Monster* _entity)
{
    if (_entity->FindOut())
    {
        _entity->GetAI()->ChangeState(MON_STATE::CHASE);
    }
}

void CommonMonState::Trace::Exit(Monster* _entity)
{
}

/* === === === === ===
*  Chase 상태
*  === === === === === */
void CommonMonState::Chase::Enter(Monster* _entity)
{
    _entity->UpdateAnimation(MON_STATE::CHASE);
}

void CommonMonState::Chase::Update(Monster* _entity)
{
}

void CommonMonState::Chase::Exit(Monster* _entity)
{
}

/* === === === === ===
*  Attack 상태
*  === === === === === */
void CommonMonState::Attack::Enter(Monster* _entity)
{
    _entity->UpdateAnimation(MON_STATE::ATTACK);
}

void CommonMonState::Attack::Update(Monster* _entity)
{
}

void CommonMonState::Attack::Exit(Monster* _entity)
{
}

/* === === === === ===
*  Die 상태
*  === === === === === */
void CommonMonState::Die::Enter(Monster* _entity)
{
    _entity->UpdateAnimation(MON_STATE::DIE);
}

void CommonMonState::Die::Update(Monster* _entity)
{
}

void CommonMonState::Die::Exit(Monster* _entity)
{
}

/* === === === === ===
*  GLOBAL 상태 검사
*  === === === === === */
void CommonMonState::Global::Enter(Monster* _entity)
{
}

void CommonMonState::Global::Update(Monster* _entity)
{
    if (_entity->PlayerLost())  // 플레이어를 잃은 상태라면
    {
        _entity->GetAI()->ChangeState(MON_STATE::IDLE);
        return;
    }
    
    if (_entity->GetMonInfo().hp <= 0 && _entity->GetAI()->GetState() != MON_STATE::DIE)  // 체력이 다하면 죽음
    {
        _entity->GetAI()->ChangeState(MON_STATE::DIE);
        return;
    }
}

void CommonMonState::Global::Exit(Monster* _entity)
{
}
