#include "Framework.h"
#include "Background.h"

#include "Engine/Resource/Shader.h"

Background::Background(Texture* _texture, Shader* _shader, Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
    Background(_texture, _shader, { 1.0f, 0.0f }, _scene, _type, _updateOrder, _parent)
{}

Background::Background(Texture* _texture, Shader* _shader, D3DXVECTOR2 _scrollSpeed, Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
    GameObject(_scene, _type, _updateOrder, _parent)
    , m_shader(_shader)
    , m_texture(_texture)
    , m_scrollSpeed(_scrollSpeed)
    , m_beforeCameraPos(CAMERA->GetPos())
    , m_afterCameraPos(CAMERA->GetPos())
{
    m_textureSize = m_texture->GetSize();
    m_textureHalfSize = m_texture->GetHalfSize();
}

Background::~Background() = default;

void Background::UpdateObject()
{
    m_beforeCameraPos = m_afterCameraPos;
    m_afterCameraPos = CAMERA->GetPos();

    D3DXVECTOR2 cameraMove = m_afterCameraPos - m_beforeCameraPos;
    cameraMove.x *= m_scrollSpeed.x;
    cameraMove.y *= m_scrollSpeed.y;
    
    AddPos(cameraMove);
}

void Background::RenderObject()
{
    GameObject::RenderObject();

    m_shader->Begin();
    m_texture->Render();
    m_shader->End();
}

/* === === === === ===
*  Getter / Setter
* === === === === === */

inline FRECT Background::Rect() 
{ 
    D3DXVECTOR2 pos = GetWorldPos(); 
    D3DXVECTOR2 scale = GetWorldScale(); 
    
    return FRECT(
        pos.x - (m_textureHalfSize.x * scale.x),
        pos.y - (m_textureHalfSize.y * scale.y),
        pos.x + (m_textureHalfSize.x * scale.x),
        pos.y + (m_textureHalfSize.y * scale.y));
}

inline D3DXVECTOR2 Background::Size() 
{ 
    D3DXVECTOR2 scale = GetWorldScale(); 
    return { m_textureSize.x * scale.x, m_textureSize.y * scale.y }; 
}

