#pragma once
class Monster;

enum class MONSTERS
{
    MMM,
    FIN
};

class MonsterFactory
{
private:
    MonsterFactory() = default;
    ~MonsterFactory() = default;

public:
    static Monster* CreateMonster(Scene* _scene, MONSTERS _monsterType, D3DXVECTOR2 _pos, GameObject* _parent = nullptr);
};

