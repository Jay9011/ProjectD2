#include "Framework.h"
#include "Bullet.h"

#include "Engine/Resource/Shader.h"
#include "Engine/Component/Component.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Component/Collision/Colliders/AARect.h"

Bullet::Bullet(float _limitDist, float _damage, float _speed, Scene* _scene, int _updateOrder) :
    GameObject(_scene, _updateOrder, nullptr)
    , m_speed(_speed)
    , m_damage(_damage)
    , m_dir({ 0, 0 })
    , m_startPos({ 0, 0 })
    , m_limitDist(_limitDist)
{
    m_animator = ADDCOMP::NewAnimator(this, SHADER(L"AlphaShader"));
    m_animator->LoadXML("SFX\\Projectiles\\", "Bullet01", ANIM_PLAY_TYPE::LOOP, 0.1f);
    
    m_collider = ADDCOMP::NewAARect({ -5, -4 }, { 5, 4 }, this);

    Active(false);
}

Bullet::~Bullet() = default;

void Bullet::Fire(D3DXVECTOR2 _dir, float _damage, float _speed)
{
    m_dir = _dir;
    
    if(_damage != 0.0f)
        m_damage = _damage;
    
    if(_speed != 0.0f)
        m_speed = _speed;

    m_startPos = GetPos();  // 시작 지점 기록

    Active(true);
}

void Bullet::UpdateObject()
{
    D3DXVECTOR2 pos = GetPos();
    D3DXVECTOR2 dist = pos - m_startPos;

    if(D3DXVec2LengthSq(&dist) >= m_limitDist * m_limitDist)
    {
        Active(false);
        return;
    }
    
    AddPos(m_dir * (m_speed * fDT));
}

void Bullet::Active(bool _b)
{
    if (_b)
    {
        m_collider->IsActive(true);
        SetState(OBJECT_STATE::ACTIVE);
        m_animator->Find(0)->Play();

        D3DXVECTOR2 scale = GetScale();
        if(m_dir.x < 0 && scale.x > 0)
            SetScale({ -scale.x, scale.y });
        else if(m_dir.x > 0 && scale.x < 0)
            SetScale({ -scale.x, scale.y });
    }
    else
    {
        SetPos(-5000.0f, -5000.0f);
        
        m_collider->IsActive(false);
        SetState(OBJECT_STATE::INACTIVE);
        m_animator->Find(0)->Reset();
    }
}
