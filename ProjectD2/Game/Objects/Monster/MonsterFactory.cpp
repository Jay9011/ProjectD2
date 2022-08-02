#include "Framework.h"
#include "MonsterFactory.h"

#include "Monster.h"
#include "Monsters.h"

Monster* MonsterFactory::CreateMonster(Scene* _scene, MONSTERS _monsterType, D3DXVECTOR2 _pos, GameObject* _parent)
{
    Monster* monster = nullptr;
    MonInfo info = {};
    Physics physics = {};
    
    switch (_monsterType)
    {
    case MONSTERS::MMM:
        monster = new MMM(_scene, OBJECT_TYPE::MONSTER, 110, _parent);
        monster->SetPos(_pos);
        info.hp = 5.0f;
        info.attack = 1.0f;
        info.attackRange = 50.0f;
        physics.owner = monster;
        physics.speed = 200.0f;
        break;
    default:
        break;
    }
    
    if (monster != nullptr)
    {
        monster->SetMonInfo(info);
        monster->SetPhysics(physics);
    }
    
    return monster;
}
