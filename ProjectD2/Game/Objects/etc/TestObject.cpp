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
	, m_speed(300.f)
	, m_state(PLAYER_STATE::APPEAR)
{
#if _DEBUG
	SetDrawDirection(true);
	TwAddVarRW(_scene->twbar, "Speed", TW_TYPE_FLOAT, &m_speed, "min=0.0 max=1000.0 step=1.0");
	TwAddVarRO(_scene->twbar, "Dir", TW_TYPE_DIR3F, &m_dir, "opened=true axisy=-y");
	TwAddVarRW(_scene->twbar, "Force", TW_TYPE_DIR3F, &m_physics.force, "opened=true axisy=-y");
	TwAddVarRO(_scene->twbar, "isFalling", TW_TYPE_BOOL8, &m_physics.isFalling, "");
	TwAddVarRO(_scene->twbar, "isWallSliding", TW_TYPE_BOOL8, &m_physics.isWallSliding, "");
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
	m_handCollider = ADDCOMP::NewAARect({0, -15}, {25, 0}, this);
	m_handCollider->SetCallbackOnCollisionEnter([&](Collider* _other) 
		{ 
			if (_other->options.slidable)
			{
				std::cout << "Hand HoldOn!!!" << std::endl;
                m_physics.resistance.y = _other->options.resistance.y;
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
	
	// Animation IDLE 화
	if (KEYUP(VK_LEFT) || KEYUP(VK_RIGHT) || KEYUP(VK_UP) || KEYUP(VK_DOWN))
		SetAction(PLAYER_STATE::IDLE);
	
	// 최종 위치 변환
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
	HandPlatformCheck();
	BodyPlatformCheck();
}

void TestObject::FinalUpdateObject()
{
	/*
	* 마무리 처리
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

	if (KEYDOWN(VK_UP))
	{
		m_physics.Jump();
	}
}

void TestObject::BodyPlatformCheck()
{
	// 충돌중인 모든 Platform을 찾는다.
	vector<std::pair<Collider*, Collider*>> collided;
	scene->GetCollisionMgr()->CheckCollision(m_bodyCollider, OBJECT_TYPE::PLATFORM, collided);
	
    // 기본적으로 모든 상황에 대비해 Falling을 켜준다.
	m_physics.isFalling = true;
	
	if(!collided.empty())
	{
		D3DXVECTOR2 correctPos = { 0, 0 };
		bool isBumpWall = false;
		SIDE side = SIDE::NONE;
        
		// 모든 충돌체에 대한 조건 처리
		for (const auto& collider : collided)
		{
			side = m_physics.CollisionCorrect(correctPos, m_bodyCollider, collider.second);	// 충돌 방향과 크기 체크
            
			// 좌우에서 이번에 부딪힌 경우
			if (correctPos.x != 0 && collider.second->GetState() == COLLISION_STATE::ENTER)
			{
				isBumpWall = true;
			}

			// 땅에 닿는 순간
			if (side == SIDE::UPPER_SIDE && collider.second->GetState() == COLLISION_STATE::ENTER)
			{
				m_physics.JumpReset();	// 점프 초기화
			}
		}

		// 위치 보정
		if (correctPos.x != 0 || correctPos.y != 0)
		{
            AddPos(correctPos);
		}

        // 벽에 부딪혔을 때 위로 올라가고 있었다면 올라가는 힘을 줄인다.
		if (isBumpWall && m_physics.force.y < 0)
		{
			m_physics.force.y -= (m_physics.force.y * 0.3f);
		}
		
	}
}

void TestObject::HandPlatformCheck()
{
	// 충돌중인 모든 Platform을 찾는다.
	vector<std::pair<Collider*, Collider*>> collided;
	scene->GetCollisionMgr()->CheckCollision(m_handCollider, OBJECT_TYPE::PLATFORM, collided);

	bool wallSliding = false;
    
    // 만약 잡고있는 벽이 있다면
	if (!collided.empty())
	{
		// 모든 충돌체에 대해 슬라이딩이 가능한 충돌체에 HandCollider가 닿아있는지 확인
		for (auto& platform : collided)
		{
            // 하나라도 slidable인 벽이 있으면
			if (platform.second->options.slidable == true)
			{
                if(m_physics.force.y > 0)
				{
					wallSliding = true;
				}
				else
				{
					wallSliding = false;
				}
			}
			// slidable 벽이 없는 경우
			else
			{
				wallSliding = false;
			}
		}
	}

	if (wallSliding)
	{
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