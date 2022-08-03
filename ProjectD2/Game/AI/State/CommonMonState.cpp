#include "Framework.h"
#include "CommonMonState.h"

#include "Game/Objects/Monster/Monster.h"

/* === === === === === 
*  IDLE ����
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
*  Patrol ����
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
*  Trace ����
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
*  Chase ����
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
*  Attack ����
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
*  Die ����
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
*  GLOBAL ���� �˻�
*  === === === === === */
void CommonMonState::Global::Enter(Monster* _entity)
{
}

void CommonMonState::Global::Update(Monster* _entity)
{
    if (_entity->PlayerLost())  // �÷��̾ ���� ���¶��
    {
        _entity->GetAI()->ChangeState(MON_STATE::IDLE);
        return;
    }
    
    if (_entity->GetMonInfo().hp <= 0 && _entity->GetAI()->GetState() != MON_STATE::DIE)  // ü���� ���ϸ� ����
    {
        _entity->GetAI()->ChangeState(MON_STATE::DIE);
        return;
    }
}

void CommonMonState::Global::Exit(Monster* _entity)
{
}
