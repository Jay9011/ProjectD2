#include "Framework.h"
#include "UIObject.h"

#include "Engine/Component/Component.h"

UIObject::UIObject(Scene* _scene, int _updateOrder, UIObject* _parent) :
    Transform(_parent)
    , m_scene(_scene)
    , m_state(UI_STATE::ACTIVE)
    , m_updateOrder(_updateOrder)
{
    m_scene->AddUI(this);
}

UIObject::~UIObject()
{
    /*     Component Release       */
    while (!m_componentList.empty())
    {
        m_componentList.back()->Delete();
    }

    /*     GameObject Release        */
    m_scene->DeleteUI(this);
}

void UIObject::Update()
{
	if (m_isUpdating)
		return;

	m_isUpdating = true;

	if (m_state == UI_STATE::ACTIVE)
	{
		UpdateWorld();

		UpdateObject();
		UpdateComponent();
		PostUpdateObject();

		UpdateWorld();
	}

	m_isUpdating = false;
}

void UIObject::UpdateComponent()
{
	for (auto& component : m_componentList)
	{
		component->Update();
	}
}

void UIObject::FinalUpdate()
{
	if (m_isUpdating)
		return;

	m_isUpdating = true;

	if (m_state == UI_STATE::ACTIVE)
	{
		FinalUpdateComponent();
		FinalUpdateObject();
	}

	m_isUpdating = false;
}

void UIObject::FinalUpdateComponent()
{
	for (auto& component : m_componentList)
	{
		component->FinalUpdate();
	}
}

void UIObject::Render()
{
	if (m_isRendering)
		return;

	m_isRendering = true;

	SetWorld();
	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	DEVICE->SetTransform(D3DTS_VIEW, &view);
    
	RenderComponent();
	RenderObject();

	m_isRendering = false;
}

void UIObject::RenderComponent()
{
	for (auto& component : m_componentList)
	{
		if (!component->AutoRendering())
			return;

		component->Render();
	}
}

void UIObject::AddComponent(Component* _component)
{
	int componentOrder = _component->GetUpdateOrder();
	auto iter = m_componentList.begin();
	for (; iter != m_componentList.end(); iter++)
	{
		if (componentOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}

	m_componentList.insert(iter, _component);
}

void UIObject::RemoveComponent(Component* _component)
{
	auto iter = std::find(m_componentList.begin(), m_componentList.end(), _component);
	if (iter != m_componentList.end())
	{
		m_componentList.erase(iter);
	}
}
