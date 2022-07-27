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
	, m_state(PLAYER_STATE::APPEAR)
{
#if _DEBUG
	SetDrawDirection(true);
	TwAddVarRW(_scene->twbar, "Speed", TW_TYPE_FLOAT, &m_physics.speed, "min=0.0 max=1000.0 step=1.0");
	TwAddVarRO(_scene->twbar, "Dir", TW_TYPE_DIR3F, &m_dir, "opened=true axisy=-y");
	TwAddVarRW(_scene->twbar, "Force", TW_TYPE_DIR3F, &m_physics.force, "opened=true axisy=-y");
	TwAddVarRO(_scene->twbar, "isFalling", TW_TYPE_BOOL8, &m_physics.isFalling, "");
	TwAddVarRO(_scene->twbar, "isWallSliding", TW_TYPE_BOOL8, &m_physics.isWallSliding, "");
	TwAddVarRW(_scene->twbar, "WallResistance", TW_TYPE_FLOAT, &m_physics.resistance.y, "");
	TwAddVarRO(_scene->twbar, "JumpCount", TW_TYPE_INT16, &m_physics.jumpCount, "");
	TwAddVarRO(_scene->twbar, "PressedJumpTime", TW_TYPE_DOUBLE, &pressedJumpKey, "");
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
	m_bodyCollider->SetCallbackOnCollisionEnter([](Collider* _other) { std::cout << "BodyEnter - " + _other->GetTag() << std::endl; });
    m_bodyCollider->SetCallbackOnCollisionExit([](Collider* _other) { std::cout << "BodyExit  - " + _other->GetTag() << std::endl; });
	m_bodyCollider->IsActive(true);
	m_bodyCollider->SetTag("body");
	m_handCollider = ADDCOMP::NewAARect({0, -15}, {18, 0}, this);
	m_handCollider->SetCallbackOnCollisionEnter([&](Collider* _other) 
		{ 
			if (_other->options.slidable)
			{
				std::cout << "Hand HoldOn!!!" << std::endl;
			}
		});
	m_handCollider->SetCallbackOnCollisionExit([&](Collider* _other)
		{
			if (_other->options.slidable)
			{
				std::cout << "Exit Hand!!!" << std::endl;
			}
		});
	m_handCollider->IsActive(true);
	m_handCollider->SetTag("hand");
	/*
	* Physics
	*/
	m_physics.owner = this;
	m_physics.maxJumpCount = 2;
	m_physics.mass = 1.5;
	m_physics.speed = 300.0f;
	m_physics.jumpForce = 400.f;
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
	
	// Animation IDLE ȭ
	if (KEYUP(VK_LEFT) || KEYUP(VK_RIGHT) || KEYUP(VK_UP) || KEYUP(VK_DOWN))
		SetAction(PLAYER_STATE::IDLE);
	
	// ���� ��ġ ��ȯ
	AddPos(m_physics.force * fDT);

#if _DEBUG
	m_dir = { GetDirection().x, GetDirection().y, 0 };
#endif // _DEBUG
}

void TestObject::RenderObject()
{
	GameObject::RenderObject();
}

void TestObject::PostUpdateObject()
{
	BodyPlatformCheck();
	HandPlatformCheck();
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
	m_animator->Find((int)PLAYER_STATE::APPEAR)->SetFrameDuration(16, 1.0f);
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
	// Scale�� ���� ������ ������ �޶����� ������ Scale ���� ���� ������ ó���Ѵ�.
	if (KEYDOWN(VK_UP))
	{
		m_physics.Jump();
	}
    
	if (KEYPRESS(VK_LEFT))
	{
		m_physics.MovingX(V_LEFT.x);
		if (m_isRight)
		{
			m_isRight = !m_isRight;
			ScaleXInverse();
		}

		SetAction(PLAYER_STATE::RUN);
	}
	if (KEYPRESS(VK_RIGHT))
	{
		m_physics.MovingX(V_RIGHT.x);
		if (!m_isRight)
		{
			m_isRight = !m_isRight;
			ScaleXInverse();
		}

		SetAction(PLAYER_STATE::RUN);
	}

}

void TestObject::BodyPlatformCheck()
{
	// �浹���� ��� Platform�� ã�´�.
	vector<std::pair<Collider*, Collider*>> collided;
	scene->GetCollisionMgr()->CheckCollision(m_bodyCollider, OBJECT_TYPE::PLATFORM, collided);
	
    // �⺻������ ��� ��Ȳ�� ����� Falling�� ���ش�.
	m_physics.isFalling = true;
	
	if(!collided.empty())
	{
		D3DXVECTOR2 correctPos = { 0, 0 };
		bool isBumpWall = false;
		SIDE side = SIDE::NONE;
        
		// ��� �浹ü�� ���� ���� ó��
		for (const auto& collider : collided)
		{
			side = m_physics.CollisionCorrect(correctPos, m_bodyCollider, collider.second);	// �浹 ����� ũ�� üũ
            
			// �¿쿡�� �̹��� �ε��� ���
			if (correctPos.x != 0 && collider.second->GetState() == COLLISION_STATE::ENTER)
			{
				m_physics.force.x = 0;
				isBumpWall = true;
			}

			// ���� ��� ����
			if (side == SIDE::UPPER_SIDE && collider.second->GetState() == COLLISION_STATE::ENTER)
			{
				m_physics.JumpReset();	// ���� �ʱ�ȭ
			}
		}

		// ��ġ ����
		if (correctPos.x != 0 || correctPos.y != 0)
		{
            AddPos(correctPos);
		}

        // ���� �ε����� �� ���� �ö󰡰� �־��ٸ� �ö󰡴� ���� ���δ�.
		if (isBumpWall && m_physics.force.y < 0)
		{
			m_physics.force.y -= (m_physics.force.y * 0.3f);
		}
		
	}
}

void TestObject::HandPlatformCheck()
{
	// �浹���� ��� Platform�� ã�´�.
	vector<std::pair<Collider*, Collider*>> collided;
	scene->GetCollisionMgr()->CheckCollision(m_handCollider, OBJECT_TYPE::PLATFORM, collided);

	bool wallSliding = false;
	float resist = 0.0f;
    
    // ���� ����ִ� ���� ���� ��� ��� ��Ȳ�� ����� Resistance.y �� 0���� �ʱ�ȭ��Ų��.
	if (collided.empty())
	{
		m_physics.resistance.y = 0;
	}
	else
	{
		// ��� �浹ü�� ���� �����̵��� ������ �浹ü�� HandCollider�� ����ִ��� Ȯ��
		for (auto& platform : collided)
		{
            // �ϳ��� slidable�� ���� ������
			if (platform.second->options.slidable == true)
			{
				resist = Math::Max(resist, platform.second->options.resistance.y);

                if(m_physics.force.y > 0)
				{
					wallSliding = true;
				}
				else
				{
					wallSliding = false;
				}
			}
			// slidable ���� ���� ���
			else
			{
				wallSliding = false;
			}
		}
	}

	if (wallSliding)
	{
		m_physics.resistance.y = resist;
        
		if (!m_physics.isWallSliding)
			m_physics.WallSlidingStart();
		else
			m_physics.WallSlidingStay();
	}
	else if(m_physics.isWallSliding)
	{
		m_physics.WallSlidingEnd();
	}

}