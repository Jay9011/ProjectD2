#pragma once
#include "Engine/Object/GameObject.h"

class Part : public GameObject
{
public:
    Part(Scene* _scene, int _updateOrder = 100, GameObject* _parent = nullptr);
    ~Part();

    // GameObject��(��) ���� ��ӵ�
    virtual void UpdateObject() override;
};

