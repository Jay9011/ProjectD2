#include "Framework.h"
#include "WeaponUI.h"

#include "Engine/Component/Component.h"
#include "Engine/Resource/Shader.h"
#include "Engine/Component/Animator/Animator.h"

#include "Game/Objects/Charactor/Player.h"

WeaponUI::WeaponUI(Scene* _scene, int _updateOrder, UIObject* _parent) :
    UIObject(_scene, _updateOrder, _parent)
	, m_scene(_scene)
{
	m_animator = ADDCOMP::NewAnimator(this, SHADER(L"AlphaShader"));
	SetAnimation();
	SetPos({ 80.0f, 50.0f });
	SetScale(0.5f, 0.5f);

	m_scene->GetGame()->playerObservable->Subscribe(this);
}

WeaponUI::~WeaponUI()
{
}

void WeaponUI::UpdateObject()
{
}

void WeaponUI::RenderObject()
{
	
}

void WeaponUI::FieldChanged(Player& field, const string& fieldName)
{
	if (fieldName == "GetEquip")
	{
		PLAYER_EQUIP_TYPE equipType = field.GetEquip();
		PlayAnimation(equipType);
	}
}

void WeaponUI::SetAnimation()
{
	for (int i = 0; i < 2; i++)
	{
		switch ((PLAYER_EQUIP_TYPE)i)
		{
		case PLAYER_EQUIP_TYPE::GUN:
			m_animator->LoadXML("UI\\", "GunChange", ANIM_PLAY_TYPE::ONCE, 0.05f);
			break;
		case PLAYER_EQUIP_TYPE::SWORD:
			m_animator->LoadXML("UI\\", "SwordChange", ANIM_PLAY_TYPE::ONCE, 0.05f);
			break;
		}
	}
}

void WeaponUI::PlayAnimation(PLAYER_EQUIP_TYPE _type)
{
	m_animator->Find((int)_type)->Play();
}