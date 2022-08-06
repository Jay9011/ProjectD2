#pragma once
#include "Engine/Object/Transform.h"
class Shader;
class Animation;
class Effect : public Transform
{
public:
    Effect(const wstring& _path, int _maxFrameX, int _maxFrameY, ANIM_PLAY_TYPE _type = ANIM_PLAY_TYPE::ONCE, bool _isAdditive = true, float speed = 0.1f);
    ~Effect() override;

    void Update();
    void FinalUpdate();
    void Render();

    void Play(D3DXVECTOR2 _pos, D3DXCOLOR _color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
    void Stop();

private:
    Shader* m_shader;
    Animation* m_animation;
    ANIM_PLAY_TYPE m_type;

    bool isActive;
    bool isAdditive;

    D3DXCOLOR m_color;

public:
    bool IsActive() const  { return isActive; }
    void IsActive(bool _b) { isActive = _b; }
};

