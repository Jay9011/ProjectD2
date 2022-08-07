#include "Framework.h"
#include "Image.h"

#include "Engine/Resource/Shader.h"

Image::Image(Texture* _texture, Scene* _scene, int _updateOrder, UIObject* _parent) :
    UIObject(_scene, _updateOrder, _parent)
    , m_texture(_texture)
    , m_shader(nullptr)
{
}

Image::~Image()
{
}

void Image::UpdateObject()
{
}

void Image::RenderObject()
{
    if (!m_rendering)
        return;

    if (m_shader != nullptr)
        m_shader->Begin();

    if (m_texture != nullptr)
    {
        m_texture->Render();
    }
    
    if (m_shader != nullptr)
        m_shader->End();
}
