#pragma once
#include <queue>
#include "Engine/Object/Transform.h"

enum class CAM_EFFECT
{
    SHAKE,
    FIN
};

struct tagCamEffect
{
    CAM_EFFECT effect;
    float      duration;
    float      progTime;
    float      power;
};

class Camera : public Transform
{
public:
    Camera();
    ~Camera();
    
public:
    void Update();

    void FreeMode();
    void TargetMode();
  
private:
    float m_speed;
    
    Transform* m_target;
    
    D3DXVECTOR2 m_offset;
    FRECT m_restrictRange;
    
    std::queue<tagCamEffect> m_effectQueue;

public:
    /* === === === === === 
    *  Camera Effects
    *  === === === === === */
    void Shake(float _duration, float _power)
    {
        if (_duration <= 0) assert(0);
        
        tagCamEffect effect;
        effect.effect   = CAM_EFFECT::SHAKE;
        effect.duration = _duration;
        effect.progTime = 0.0f;
        effect.power    = _power;
        
        m_effectQueue.push(effect);
    }

public:
    /* === === === === === 
    *  Getter / Setter
    *  === === === === === */
    void SetTarget(Transform* _target) { m_target = _target; }
    
    void SetRestrictRange(FRECT _rect) { m_restrictRange = _rect; }
    void SetRestrictRange(float _left, float _top, float _right, float _bottom) { m_restrictRange = FRECT(_left, _top, _right, _bottom); }
    
};

