#pragma once
#include "Engine/Object/UIObject.h"

class TestUI_1 :public UIObject
{
public:
    TestUI_1(Scene* _scene, int _updateOrder = 100, UIObject* _parent = nullptr);
    ~TestUI_1() override;

    // UIObject을(를) 통해 상속됨
    virtual void UpdateObject() override;
    virtual void RenderObject() override;

private:

    void SetVertex();
};

