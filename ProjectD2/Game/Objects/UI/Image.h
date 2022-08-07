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
    Shader*  m_shader;
    Texture* m_texture;
    
private:
    Scene* m_scene;
    bool m_rendering = true;

public:
    void SetRendering(bool _rendering) { m_rendering = _rendering; }
    
    void SetShader(Shader* _shader) { m_shader = _shader; }
};

