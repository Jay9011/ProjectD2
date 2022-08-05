#include "Framework.h"
#include "DialogEvent.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/Collision/Collision.h"

DialogEvent::DialogEvent(D3DXVECTOR2 _colMin, D3DXVECTOR2 _colMax, Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
    GameObject(_scene, _type, _updateOrder, _parent)
{
    m_collider = ADDCOMP::NewAARect(_colMin, _colMax, _type, this);
    m_collider->IsActive(true);
}

DialogEvent::~DialogEvent()
{
}

void DialogEvent::UpdateObject()
{
}

void DialogEvent::RenderObject()
{
}
