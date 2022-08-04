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
    
    // 기본적으로 하트를 1개 생성해둔다. (사용하기 위함)
    //m_hearts.emplace_back(new Heart(m_scene, 100, this));
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
        if (m_maxHP == 0)
        {
            m_maxHP = field.GetMaxHP();
            InitHeart();
        }
    }
}

void HeartUI::InitHeart()
{
    int countX = m_maxHP;
        
    D3DXVECTOR2 pos = GetPos();
    D3DXVECTOR2 textureSize = m_hearts[0]->animator->GetCurrentTexture()->GetSize();
    D3DXVECTOR2 textureHalfSize = textureSize * 0.5f;

    //for (size_t x = 1; x < countX; x++)
    //{
    //    m_hearts.emplace_back(new Heart(m_scene, 100, this));
    //    m_hearts[x]->SetPos(pos.x + textureSize.x, 0);
    //    pos = m_hearts[x]->GetPos();
    //}
}
//
//Heart::Heart(Scene* _scene, int _updateOrder, UIObject* _parent) :
//    UIObject(_scene, _updateOrder, _parent)
//{
//    animator = ADDCOMP::NewAnimator(this, SHADER(L"AlphaShader"));
//
//    for (int i = 0; i < (int)HEART_STATE::FIN; i++)
//    {
//        switch ((HEART_STATE)i)
//        {
//        case HEART_STATE::Empty:
//            animator->LoadXML("UI\\", "heartEmpty", ANIM_PLAY_TYPE::ONCE, 0.05f);
//            break;
//        case HEART_STATE::Full:
//            animator->LoadXML("UI\\", "heart", ANIM_PLAY_TYPE::ONCE, 0.05f);
//            break;
//        default:
//            break;
//        }
//    }
//
//    animator->SetCurrentAnimation(animator->Find((int)HEART_STATE::Full));
//}
//
//Heart::~Heart()
//{
//}
//
//void Heart::UpdateObject()
//{
//}
//
//void Heart::RenderObject()
//{
//}
