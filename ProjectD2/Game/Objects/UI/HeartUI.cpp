#include "Framework.h"
#include "HeartUI.h"

#include "Engine/Resource/Shader.h"
#include "Engine/Component/Component.h"
#include "Engine/Component/Animator/Animator.h"

#include "Game/Objects/Charactor/Player.h"

HeartUI::HeartUI(Scene* _scene, int _updateOrder, UIObject* _parent) :
    UIObject(_scene, _updateOrder, _parent)
    , m_scene(_scene)
{
    m_scene->GetGame()->playerObservable->Subscribe(this);

    m_fullHeart = TEXTURE->Add(L"UI\\Heart.png");
    m_emptyHeart = TEXTURE->Add(L"UI\\EmptyHeart.png");
    m_shader = SHADER(L"AlphaShader");
}

HeartUI::~HeartUI()
{
    Observable<Player>* playerObservable = m_scene->GetGame()->playerObservable;
    if (playerObservable != nullptr)
        playerObservable->Unsubscribe(this);
}

void HeartUI::UpdateObject()
{
    
    
}

void HeartUI::RenderObject()
{
    D3DXVECTOR2 pos = GetWorldPos();
    D3DXVECTOR2 scale = GetWorldScale();
    D3DXVECTOR2 textureSize = m_fullHeart->GetSize();

    m_shader->Begin();
    for (int i = 0; i < m_maxHP; i++)
    {
        SetPos(pos.x + (scale.x * textureSize.x * i), pos.y);
        UpdateWorld();
        SetWorld();
        
        if (i < m_HP)
        {
            m_fullHeart->Render();
        }
        else
        {
            m_emptyHeart->Render();
        }
    }
    m_shader->End();
    
    SetPos(pos);
    UpdateWorld();
    SetWorld();
}

void HeartUI::FieldChanged(Player& field, const string& fieldName)
{
    if (fieldName == "GetHP")
    {
        m_HP = field.GetHP();
    }
    else if (fieldName == "GetMaxHP")
    {
        if (m_maxHP == 0)
        {
            m_maxHP = field.GetMaxHP();
        }
    }
}