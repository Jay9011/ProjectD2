#include "Framework.h"
#include "UIObject.h"

#include "Engine/Component/Component.h"

UIObject::UIObject(Scene* _scene, int _updateOrder, GameObject* _parent) :
    GameObject(_scene, OBJECT_TYPE::UI, _updateOrder, _parent)
    , m_scene(_scene)
{
	m_scene->AddUI(this);
}

UIObject::~UIObject()
{
	m_scene->DeleteUI(this);
}