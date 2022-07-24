#include "Framework.h"
#include "TestObject.h"

#include "Engine/Resource/Shader.h"
#include "Engine/Component/Component.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Component/Collision/Collision.h"
#include "Engine/Component/PhysicsWorld/Physics.h"

TestObject::TestObject(Scene* _scene, int _updateOrder, GameObject* _parent) :
	TestObject(_scene, OBJECT_TYPE::DEFAULT, _updateOrder, _parent)
{}

TestObject::TestObject(Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, _type, _updateOrder, _parent)
	, m_isRight(true)
	, m_speed(200.f)
	, m_state(PLAYER_STATE::APPEAR)
{
#if _DEBUG
	TwAddVarRW(_scene->twbar, "Speed", TW_TYPE_FLOAT, &m_speed, "min=0.0 max=1000.0 step=1.0");
#endif // _DEBUG
	/* === === === === ===
	*  Component Setting
	* === === === === === */
	/*
	* Animator
	*/
	m_animator = ADDCOMP::NewAnimator(this, SHADER(L"AlphaShader"));
	/*
	* Collider
	*/
	m_bodyCollider = ADDCOMP::NewAARect({ -15, -17 }, { 0, 21 }, this);
	m_bodyCollider->IsActive(true);
	m_sight_u = ADDCOMP::NewLine({ 0, 0 }, { 150, -70 }, this);
	m_sight_u->IsActive(true);
	
	/*
	* Physics
	*/
	ADDCOMP::NewGravity(m_physics, this);

	SetAnimation();
	m_animator->Find((int)PLAYER_STATE::APPEAR);
}

TestObject::~TestObject() = default;

void TestObject::UpdateObject()
{
	D3DXVECTOR3 pos = GetPos();
	SetPos(pos);
	
	/*Collision(m_bodyCollider, MOUSEPOS);
	Collision(m_sight_u, MOUSEPOS);*/

	if(KEYPRESS(VK_LEFT))
	{
		AddPos(V_LEFT * m_speed * fDT);
		if (m_isRight)
		{
			m_isRight = !m_isRight;
			ScaleXInverse();
		}

		SetAction(PLAYER_STATE::RUN);
	}
	if (KEYPRESS(VK_RIGHT))
	{
		AddPos(V_RIGHT * m_speed * fDT);
		if (!m_isRight)
		{
			m_isRight = !m_isRight;
			ScaleXInverse();
		}
		
		SetAction(PLAYER_STATE::RUN);
	}
	
	if (KEYPRESS(VK_UP))
	{
		AddPos(V_UP * m_speed * fDT);
		SetAction(PLAYER_STATE::RUN);
	}
	if (KEYPRESS(VK_DOWN))
	{
		AddPos(V_DOWN * m_speed * fDT);
		SetAction(PLAYER_STATE::RUN);
	}

	if (KEYUP(VK_LEFT) || KEYUP(VK_RIGHT) || KEYUP(VK_UP) || KEYUP(VK_DOWN))
		SetAction(PLAYER_STATE::IDLE);
	
	// 최종 위치 변환
	AddPos(m_physics.force * fDT);
}

void TestObject::RenderObject()
{
	GameObject::RenderObject();
}

void TestObject::SetAction(PLAYER_STATE _state)
{
	if (this->m_state == _state)
		return;
	
	this->m_state = _state;
	m_animator->Find((int)m_state)->Play();
}

void TestObject::SetAnimation()
{
	vector<Texture*> frames;
	
	// APPEAR
	m_animator->LoadXML("Character\\Player\\", "Appear", ANIM_PLAY_TYPE::ONCE, 0.1f);
	m_animator->Find((int)PLAYER_STATE::APPEAR)->SetFrameDuration(16, 3.0f);
	m_animator->SetEndEvent((int)PLAYER_STATE::APPEAR, [this]() {
		SetAction(PLAYER_STATE::IDLE);
	});

	// IDLE
	m_animator->LoadXML("Character\\Player\\", "Idle", ANIM_PLAY_TYPE::PINGPONG, 0.2f);

	// RUN
	m_animator->LoadXML("Character\\Player\\", "Run", ANIM_PLAY_TYPE::LOOP, 0.05f);
	
	// JUMP
	
}
