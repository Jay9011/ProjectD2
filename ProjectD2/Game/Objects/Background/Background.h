#pragma once
#include "Engine/Object/GameObject.h"
class Background : public GameObject
{
public:
    Background(Texture* _texture, Shader* _shader, Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
    Background(Texture* _texture, Shader* _shader, D3DXVECTOR2 _scrollSpeed, Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
    ~Background();

    // GameObject을(를) 통해 상속됨
    virtual void UpdateObject() override;
    virtual void RenderObject() override;

public:
    Shader* m_shader;

private:
    Texture* m_texture;
    D3DXVECTOR2 m_textureSize;
    D3DXVECTOR2 m_textureHalfSize;

    D3DXVECTOR2 m_scrollSpeed;

    D3DXVECTOR2 m_beforeCameraPos;
    D3DXVECTOR2 m_afterCameraPos;

public:
    /* === === === === === 
    *  Getter / Setter
    * === === === === === */
    float Left()   { return GetWorldPos().x - (m_textureHalfSize.x * GetWorldScale().x); }
    float Top()    { return GetWorldPos().y - (m_textureHalfSize.y * GetWorldScale().y); }
    float Right()  { return GetWorldPos().x + (m_textureHalfSize.x * GetWorldScale().x); }
    float Bottom() { return GetWorldPos().y + (m_textureHalfSize.y * GetWorldScale().y); }
    
    FRECT       Rect();
    D3DXVECTOR2 Size();
    D3DXVECTOR2 LocalSize()       { return { m_textureSize.x * GetScale().x, m_textureSize.y * GetScale().y }; }
    D3DXVECTOR2 TextureSize()     { return m_textureSize; }
    D3DXVECTOR2 TextureHalfSize() { return m_textureHalfSize; }

    void SetTexture(Texture* _texture) { m_texture = _texture; }
    void SetScrollSpeed(D3DXVECTOR2 _speed) { m_scrollSpeed = _speed; }
    void SetScrollSpeed(float _x, float _y = 0.0f) { m_scrollSpeed = D3DXVECTOR2(_x, _y); }
    
    void InitCameraPos() { m_beforeCameraPos = CAMERA->GetPos(); m_afterCameraPos = CAMERA->GetPos(); }
    D3DXVECTOR2 GetCameraDir() { return m_beforeCameraPos - m_afterCameraPos; }
};

