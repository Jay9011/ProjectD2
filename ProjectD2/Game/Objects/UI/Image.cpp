#include "Framework.h"
#include "Image.h"

Image::Image(Texture* _texture, Scene* _scene, int _updateOrder, UIObject* _parent) :
    UIObject(_scene, _updateOrder, _parent)
    , m_texture(_texture)
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
    if (m_texture != nullptr)
    {
        m_texture->Render();
    }
}
