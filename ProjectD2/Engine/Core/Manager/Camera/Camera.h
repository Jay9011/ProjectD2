#pragma once
#include <queue>
#include "Engine/Object/Transform.h"

class Core;

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
    void Render();

private:
    void FreeMode();
    void TargetMode();
    
    void CameraEffectProgress();

    void CameraRestrictCorrection();
  
private:
    float m_speed;
    
    Transform* m_target;
    
    D3DXVECTOR2 m_offset;
    D3DXVECTOR2 m_postOffset;
    FRECT m_restrictRange;
    
    std::queue<tagCamEffect> m_effectQueue;

    short sign = 1;
    D3DXVECTOR2 m_reducer = { 0,0 };
    
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
    void SetSpeed(float _speed) { m_speed = _speed; }

    float Left();
    float Right();
    float Top();
    float Bottom();

    void SetTarget(Transform* _target) { m_target = _target; }
    void SetOffset(const D3DXVECTOR2& _offset) { m_offset = _offset; m_postOffset = _offset; }
    
    void SetRestrictRange(FRECT _rect) { m_restrictRange = _rect; }
    void SetRestrictRange(float _left, float _top, float _right, float _bottom) { m_restrictRange = FRECT(_left, _top, _right, _bottom); }
    
    void SetLookAt(const D3DXVECTOR2& _look);

    /* === === === === === 
    *  µð¹ö±ë¿ë
    * === === === === === */
public:
    void RenderCameraRect();
};

