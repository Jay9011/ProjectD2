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
    m_animator = ADDCOMP::NewAnimator(this, SHADER(L"AlphaShader"));
    SetAnimation();

    m_scene->GetGame()->playerObservable->Subscribe(this);
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
    
}

void HeartUI::FieldChanged(Player& field, const string& fieldName)
{
    if (fieldName == "GetHP")
    {
        m_HP = field.GetHP();
    }
    else if (fieldName == "GetMaxHP")
    {
        m_maxHP = field.GetMaxHP();
    }
}

void HeartUI::SetAnimation()
{
    for (int i = 0; i < Heart::FIN; i++)
    {
        switch (i)
        {
        case HeartUI::Full:
            m_animator->LoadXML("UI\\", "heart", ANIM_PLAY_TYPE::ONCE, 0.05f);
            break;
        case HeartUI::Empty:
            m_animator->LoadXML("UI\\", "heartEmpty", ANIM_PLAY_TYPE::ONCE, 0.05f);
            break;
        default:
            break;
        }
    }
}
