#include "Framework.h"
#include "TestObject.h"

#include "Engine/Singleton/Shader/Shader.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Component/Collision/Collider.h"
#include "Engine/Component/Collision/Colliders/AARect.h"

TestObject::TestObject(Scene* _scene, GameObject* _parent) :
	TestObject(_scene, OBJECT_TYPE::DEFAULT, _parent)
{}

TestObject::TestObject(Scene* _scene, OBJECT_TYPE _type, GameObject* _parent) :
	GameObject(_scene, _type, _parent)
	, m_isRight(true)
	, m_speed(200.f)
	, m_state(PLAYER_STATE::APPEAR)
{
#if _DEBUG
	TwAddVarRW(_scene->twbar, "Speed", TW_TYPE_FLOAT, &m_speed, "min=0.0 max=1000.0 step=1.0");
#endif // _DEBUG

	SetScale(1.5f, 1.5f);

	/* === === === === ===
	*  Component Setting
	* === === === === === */
	/*
	* Animator
	*/
	m_animator = new Animator(this);
	m_animator->m_shader = SHADER(L"AlphaShader");
	/*
	* Collider
	*/
	m_bodyCollider = new AARect({-20, -17}, {0, 21}, this);
	m_bodyCollider->IsActive(true);
	
	SetAnimation();
	m_animator->Find((int)PLAYER_STATE::APPEAR);

}

TestObject::~TestObject() = default;

void TestObject::UpdateObject()
{
	if (m_bodyCollider->Intersects(MOUSEPOS))
	{
		
	}
	
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

	if (KEYUP(VK_LEFT) || KEYUP(VK_RIGHT))
		SetAction(PLAYER_STATE::IDLE);
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
