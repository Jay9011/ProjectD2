#pragma once
#include "Engine/Object/GameObject.h"
class Collider;
class DialogEvent : public GameObject
{
public:
    DialogEvent(D3DXVECTOR2 _colMin, D3DXVECTOR2 _colMax, Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
    ~DialogEvent() override;

    void UpdateObject() override;
    void RenderObject() override;

private:
    bool m_isActive = false;
    bool m_isFirst = true;
    
    Collider* m_collider;
    
public:
    bool IsFirst() const  { return m_isFirst; }
    void IsFirst(bool _b) { m_isFirst = _b; }
    Collider* GetCollider() const { return m_collider; }
};

