#pragma once
#include "Engine/Object/UIObject.h"

class Image : public UIObject
{
public:
    Image(Texture* _texture, Scene* _scene, int _updateOrder = 100, UIObject* _parent = nullptr);
    ~Image() override;

    // UIObject을(를) 통해 상속됨
    virtual void UpdateObject() override;
    virtual void RenderObject() override;

public:
    Texture* m_texture;
    
private:
    Scene* m_scene;
};

