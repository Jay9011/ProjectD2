#include "Framework.h"
#include "TestObject.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Component/Collision/Collision.h"
#include "Engine/Component/PhysicsWorld/Physics.h"
#include "Engine/Resource/Shader.h"

TestObject::TestObject(Scene* _scene, int _updateOrder, GameObject* _parent) :
	TestObject(_scene, OBJECT_TYPE::DEFAULT, _updateOrder, _parent)
{}

TestObject::TestObject(Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, _type, _updateOrder, _parent)
	, scene(_scene)
	, m_isRight(true)
	, m_speed(200.f)
	, m_state(PLAYER_STATE::APPEAR)
{
#if _DEBUG
	SetDrawDirection(true);
	TwAddVarRW(_scene->twbar, "Speed", TW_TYPE_FLOAT, &m_speed, "min=0.0 max=1000.0 step=1.0");
	TwAddVarRO(_scene->twbar, "Dir", TW_TYPE_DIR3F, &m_dir, "opened=true axisy=-y");
	TwAddVarRW(_scene->twbar, "Force", TW_TYPE_DIR3F, &m_physics.force, "opened=true axisy=-y");
	TwAddVarRW(_scene->twbar, "Collided", TW_TYPE_DIR3F, &m_collisionDir, "opened=true axisy=-y");
	TwAddVarRW(_scene->twbar, "isFalling", TW_TYPE_BOOL8, &m_physics.isFalling, "");
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
	m_bodyCollider = ADDCOMP::NewAARect({ -15, -20 }, { 15, 20 }, this);
	m_bodyCollider->IsActive(true);
	/*
	* Physics
	*/
	m_physics.owner = this;
	m_physics.jumpForce = 200;
	ADDCOMP::NewGravity(m_physics, this);

	/* === === === === ===
	*   Init Settings
	* === === === === === */
	SetAnimation();
	m_animator->Find((int)PLAYER_STATE::APPEAR);
}

TestObject::~TestObject() = default;

void TestObject::UpdateObject()
{
	MoveLeftRight();
	GroundCheck();
	
	// Animation IDLE ȭ
	if (KEYUP(VK_LEFT) || KEYUP(VK_RIGHT) || KEYUP(VK_UP) || KEYUP(VK_DOWN))
		SetAction(PLAYER_STATE::IDLE);
	
	// ���� ��ġ ��ȯ
	AddPos(m_physics.force * fDT);

#if _DEBUG
	m_dir = GetDirection();
#endif // _DEBUG
}

void TestObject::RenderObject()
{
	GameObject::RenderObject();
}

void TestObject::FinalUpdateObject()
{
	/*
	* ������ ó��
	*/
	m_physics.CalcResistance();
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

void TestObject::MoveLeftRight()
{
	if (KEYPRESS(VK_LEFT))
	{
		m_physics.MovingX(V_LEFT.x * m_speed);
		if (m_isRight)
		{
			m_isRight = !m_isRight;
			ScaleXInverse();
		}

		SetAction(PLAYER_STATE::RUN);
	}
	if (KEYPRESS(VK_RIGHT))
	{
		m_physics.MovingX(V_RIGHT.x * m_speed);
		if (!m_isRight)
		{
			m_isRight = !m_isRight;
			ScaleXInverse();
		}

		SetAction(PLAYER_STATE::RUN);
	}

	if (KEYPRESS(VK_UP))
	{
		m_physics.force += V_UP * m_speed * fDT * 10;
	}

}

void TestObject::GroundCheck()
{
	vector<std::pair<Collider*, Collider*>> collided;
	scene->GetCollisionMgr()->CheckCollision(m_bodyCollider, OBJECT_TYPE::PLATFORM, collided);
	
	if (collided.empty())
	{
		m_physics.isFalling = true;
	}
	else
	{
		FRECT cRect = GetCollisionRect((AARect*)m_bodyCollider, (AARect*)collided[0].second);

		// ���� �浹
		if (cRect.Size().x > cRect.Size().y)
		{
			m_physics.isFalling = false;

			// y�� 0.3 ���ϸ� �浹 ���� ���� ����
			bool correct = cRect.Size().y > 0.3f ? true : false;
				
			if (cRect.Pos().y < m_bodyCollider->GetRect().Pos().y)
			{
				// �Ʒ����� �浹
				m_physics.isFalling = true;
				
				if (correct)
					AddPos(0, cRect.Size().y * 0.3f);
			}
			else
			{
				// ������ �浹
				if (correct)
					AddPos(0, -cRect.Size().y * 0.3f);
			}
			if (correct)
				m_physics.force.y = 0;
		}
		// �¿� �浹
		else
		{
			if(cRect.Pos().x < m_bodyCollider->GetRect().Pos().x)
			{
				// �����ʿ��� �浹
				AddPos(cRect.Size().x, 0);
			}
			else
			{
				// ���ʿ��� �浹
				AddPos(-cRect.Size().x, 0);
			}
			
			m_physics.force.x = 0;
		}
	}
}