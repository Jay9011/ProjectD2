#include "Framework.h"
#include "Monster.h"

Monster::Monster(Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
    GameObject(_scene, _type, _updateOrder, _parent)
{
}

Monster::~Monster()
{
    if(m_AI != nullptr)
        delete m_AI;
}

void Monster::UpdateObject()
{
    if(m_AI != nullptr)
        m_AI->Update();

    StateProcessing();  // 상태 변경 후 행동
    
    AddPos(m_physics.force * fDT);
}

void Monster::PostUpdateObject()
{
    m_physics.BodyCollision(m_bodyCollider);
}

void Monster::FinalUpdateObject()
{
    m_physics.CalcResistance();
}
