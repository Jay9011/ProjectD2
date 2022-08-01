#pragma once
#include "Engine/Object/GameObject.h"

class Bullet : public GameObject
{
public:
    Bullet(float _limitDist, float _damage, float _speed, Scene* _scene, int _updateOrder = 100);
    ~Bullet();
    
    void Fire(D3DXVECTOR2 _dir, float _damage = 0.0f, float _speed = 0.0f);
    
    // GameObject을(를) 통해 상속됨
    virtual void UpdateObject() override;

private:
    float m_speed;
    float m_damage;
    D3DXVECTOR2 m_dir;

    Animator* m_animator;
    AARect*   m_collider;

    D3DXVECTOR2 m_startPos;
    float       m_limitDist;

private:
    void Active(bool _b);
    
};

