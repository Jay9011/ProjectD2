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
    , m_equip(PLAYER_EQUIP_TYPE::GUN)
{
#if _DEBUG
	SetDrawDirection(true);
    TwEnumVal playerEquipEnum[] = {
        { (int)PLAYER_EQUIP_TYPE::GUN, "Gun" },
        { (int)PLAYER_EQUIP_TYPE::SWORD, "Sword" },
    };
    TwType playerEquipType = TwDefineEnum("PlayerEquipType", playerEquipEnum, 2);
	TwAddVarRW(_scene->twbar, "Speed", TW_TYPE_FLOAT, &m_physics.speed, "min=0.0 max=1000.0 step=1.0");
	TwAddVarRO(_scene->twbar, "Equip", playerEquipType, &m_equip, NULL);
	TwAddVarRO(_scene->twbar, "Dir", TW_TYPE_DIR3F, &m_dir, "opened=true axisy=-y");
	TwAddVarRW(_scene->twbar, "Force", TW_TYPE_DIR3F, &m_physics.force, "opened=true axisy=-y");
	TwAddVarRO(_scene->twbar, "isFalling", TW_TYPE_BOOL8, &m_physics.isFalling, "");
	TwAddVarRO(_scene->twbar, "isWallSliding", TW_TYPE_BOOL8, &m_physics.isWallSliding, "");
	TwAddVarRW(_scene->twbar, "WallResistance", TW_TYPE_FLOAT, &m_physics.resistance.y, "");
	TwAddVarRO(_scene->twbar, "JumpCount", TW_TYPE_INT16, &m_physics.jumpCount, "");
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
	m_bodyCollider->SetTag("body");
	m_handCollider = ADDCOMP::NewAARect({0, -15}, {18, 0}, this);
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
	// 상태 혹은 Animation을 변경하기 전 이전 상태를 저장한다.
    m_prevEquip = m_equip;
	m_prevState = m_state;
    //////////////////////////////

	Move();
	ChangeWeapon();
	
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
	BodyPlatformCheck();
	HandPlatformCheck();

}

void TestObject::FinalUpdateObject()
{
	/*
	* 마무리 처리
	*/
	m_physics.CalcResistance();
}

void TestObject::Move()
{
	// Scale에 따라 벽점프 방향이 달라지기 때문에 Scale 변경 전에 점프를 처리한다.
	if (KEYDOWN(VK_UP))
	{
		m_physics.Jump();
		UpdateState(PLAYER_STATE::JUMP, m_equip);
		
	}
    
	if (KEYPRESS(VK_LEFT))
	{
		m_physics.MovingX(V_LEFT.x);
		if (m_isRight)
		{
			m_isRight = !m_isRight;
			ScaleXInverse();
		}

		if(!m_physics.isFalling)
			UpdateState(PLAYER_STATE::RUN, m_equip);
	}
	if (KEYPRESS(VK_RIGHT))
	{
		m_physics.MovingX(V_RIGHT.x);
		if (!m_isRight)
		{
			m_isRight = !m_isRight;
			ScaleXInverse();
		}
        
		if (!m_physics.isFalling)
			UpdateState(PLAYER_STATE::RUN, m_equip);
	}

	//if (KEYUP(VK_LEFT) || KEYUP(VK_RIGHT) || KEYUP(VK_UP) || KEYUP(VK_DOWN))
	//	UpdateState(PLAYER_STATE::IDLE, m_equip);
}

void TestObject::ChangeWeapon()
{
	if (KEYDOWN(VK_TAB))
	{
        if (m_equip == PLAYER_EQUIP_TYPE::GUN)
        {
			UpdateState(m_state, PLAYER_EQUIP_TYPE::SWORD);
        }
        else
        {
			UpdateState(m_state, PLAYER_EQUIP_TYPE::GUN);
        }
		
	}
}

void TestObject::UpdateState(PLAYER_STATE _state, PLAYER_EQUIP_TYPE _equip)
{
	if (this->m_state == _state && this->m_equip == _equip)
		return;

	this->m_state = _state;
	this->m_equip = _equip;

	UpdateAnimation();
}

void TestObject::UpdateAnimation()
{
	if(m_prevState == m_state && m_prevEquip == m_equip)
        return;
    
	switch (m_state)
	{
	case PLAYER_STATE::APPEAR:
		m_animator->Find((int)PLAYER_ANIM::APPEAR)->Play();
		break;
	case PLAYER_STATE::HIT:
        m_animator->Find((int)PLAYER_ANIM::HIT)->Play();
		break;
	case PLAYER_STATE::CRITICAL:
        m_animator->Find((int)PLAYER_ANIM::CRITICAL)->Play();
		break;
	case PLAYER_STATE::DIE:
        m_animator->Find((int)PLAYER_ANIM::CRITICAL)->Play();
		break;
	case PLAYER_STATE::IDLE:
        m_animator->Find((int)PLAYER_ANIM::IDLE)->Play();
		break;
	case PLAYER_STATE::IDLE_ATK:
		if (m_equip == PLAYER_EQUIP_TYPE::GUN)
		{
            m_animator->Find((int)PLAYER_ANIM::IDLE_ATK_GUN)->Play();
        }
        else
        {
            m_animator->Find((int)PLAYER_ANIM::IDLE_ATK_SWD)->Play();
		}
		break;
	case PLAYER_STATE::RUN:
	{
		if (m_prevState == m_state)
		{
			bool reversing = m_animator->GetCurrentAnimation()->IsReverse();
			UINT nowFrame = m_animator->GetCurrentAnimation()->GetCurrentFrame();
			if (m_equip == PLAYER_EQUIP_TYPE::GUN)
			{
				m_animator->Find((int)PLAYER_ANIM::RUN_GUN)->Play(ANIM_PLAY_FLAG::SetFrameToEnd, nowFrame, -1, reversing);
			}
			else
			{
				m_animator->Find((int)PLAYER_ANIM::RUN_SWD)->Play(ANIM_PLAY_FLAG::SetFrameToEnd, nowFrame, -1, reversing);
			}
		}
		else
		{
			if (m_equip == PLAYER_EQUIP_TYPE::GUN)
			{
				m_animator->Find((int)PLAYER_ANIM::RUN_GUN)->Play();
			}
			else
			{
				m_animator->Find((int)PLAYER_ANIM::RUN_SWD)->Play();
			}
		}
	}
		break;
	case PLAYER_STATE::RUN_ATK:
        if (m_equip == PLAYER_EQUIP_TYPE::GUN)
        {
            m_animator->Find((int)PLAYER_ANIM::RUN_ATK_GUN)->Play();
        }
        else
        {
            m_animator->Find((int)PLAYER_ANIM::RUN_ATK_SWD)->Play();
        }
		break;
	case PLAYER_STATE::JUMP:
	{
		bool reversing = m_animator->GetCurrentAnimation()->IsReverse();
		UINT nowFrame = m_animator->GetCurrentAnimation()->GetCurrentFrame();
        if (m_equip == PLAYER_EQUIP_TYPE::GUN)
        {
            m_animator->Find((int)PLAYER_ANIM::JUMP_GUN)->Play();
        }
        else
        {
            m_animator->Find((int)PLAYER_ANIM::JUMP_SWD)->Play();
        }
	}
		break;
	case PLAYER_STATE::JUMP_ATK:
        if (m_equip == PLAYER_EQUIP_TYPE::GUN)
        {
            m_animator->Find((int)PLAYER_ANIM::JUMP_ATK_GUN)->Play();
        }
        else
        {
            m_animator->Find((int)PLAYER_ANIM::JUMP_ATK_SWD)->Play();
        }
		break;
	case PLAYER_STATE::HANG:
		m_animator->Find((int)PLAYER_ANIM::HANG)->Play();
		break;
	case PLAYER_STATE::HANG_ATK:
        if(m_equip == PLAYER_EQUIP_TYPE::GUN)
        {
            m_animator->Find((int)PLAYER_ANIM::HANG_ATK_GUN)->Play();
        }
        else
        {
            m_animator->Find((int)PLAYER_ANIM::HANG_ATK_SWD)->Play();
        }
		break;
	default:
		break;
	}
}

void TestObject::SetAnimation()
{
	for (int i = 0; i < (int)PLAYER_ANIM::FIN; i++)
	{
		switch ((PLAYER_ANIM)i)
		{
		case PLAYER_ANIM::APPEAR:
			m_animator->LoadXML("Character\\Player\\", "Appear", ANIM_PLAY_TYPE::ONCE, 0.1f);
			m_animator->Find((int)PLAYER_STATE::APPEAR)->SetFrameDuration(16, 0.5f);
			m_animator->SetEndEvent((int)PLAYER_STATE::APPEAR, [this]() {
				UpdateState(PLAYER_STATE::IDLE, m_equip);
				});
			break;
		case PLAYER_ANIM::HIT:
			m_animator->LoadXML("Character\\Player\\", "Hit", ANIM_PLAY_TYPE::ONCE, 0.1f);
			break;
		case PLAYER_ANIM::CRITICAL:
			m_animator->LoadXML("Character\\Player\\", "Critial", ANIM_PLAY_TYPE::ONCE, 0.1f);
			break;
		case PLAYER_ANIM::IDLE:
			m_animator->LoadXML("Character\\Player\\", "Idle", ANIM_PLAY_TYPE::PINGPONG, 0.2f);
			break;
		case PLAYER_ANIM::IDLE_ATK_GUN:
			m_animator->LoadXML("Character\\Player\\GUN\\", "IdleAttack", ANIM_PLAY_TYPE::ONCE, 0.1f);
			break;
		case PLAYER_ANIM::IDLE_ATK_SWD:
			m_animator->LoadXML("Character\\Player\\SWD\\", "IdleAttack", ANIM_PLAY_TYPE::ONCE, 0.1f);
			break;
		case PLAYER_ANIM::RUN_GUN:
			m_animator->LoadXML("Character\\Player\\GUN\\", "Run", ANIM_PLAY_TYPE::LOOP, 0.05f);
			break;
		case PLAYER_ANIM::RUN_SWD:
			m_animator->LoadXML("Character\\Player\\SWD\\", "Run", ANIM_PLAY_TYPE::LOOP, 0.05f);
			break;
		case PLAYER_ANIM::RUN_ATK_GUN:
			m_animator->LoadXML("Character\\Player\\GUN\\", "RunAttack", ANIM_PLAY_TYPE::ONCE, 0.05f);
			break;
		case PLAYER_ANIM::RUN_ATK_SWD:
			m_animator->LoadXML("Character\\Player\\SWD\\", "IdleAttack", ANIM_PLAY_TYPE::ONCE, 0.05f);
			break;
		case PLAYER_ANIM::JUMP_GUN:
			m_animator->LoadXML("Character\\Player\\GUN\\", "Jump", ANIM_PLAY_TYPE::ONCE, 0.05f);
			break;
		case PLAYER_ANIM::JUMP_SWD:
			m_animator->LoadXML("Character\\Player\\SWD\\", "Jump", ANIM_PLAY_TYPE::ONCE, 0.05f);
			break;
		case PLAYER_ANIM::JUMP_ATK_GUN:
			m_animator->LoadXML("Character\\Player\\GUN\\", "JumpAttack", ANIM_PLAY_TYPE::ONCE, 0.05f);
			break;
		case PLAYER_ANIM::JUMP_ATK_SWD:
			m_animator->LoadXML("Character\\Player\\SWD\\", "JumpAttack", ANIM_PLAY_TYPE::ONCE, 0.05f);
			break;
		case PLAYER_ANIM::HANG:
			m_animator->LoadXML("Character\\Player\\", "HoldOn", ANIM_PLAY_TYPE::ONCE, 0.05f);
			break;
		case PLAYER_ANIM::HANG_ATK_GUN:
			m_animator->LoadXML("Character\\Player\\GUN\\", "HoldOnAttack", ANIM_PLAY_TYPE::ONCE, 0.05f);
			break;
		case PLAYER_ANIM::HANG_ATK_SWD:
			m_animator->LoadXML("Character\\Player\\SWD\\", "HoldOnAttack", ANIM_PLAY_TYPE::ONCE, 0.05f);
			break;
		default:
			// Animation 등록 갯수가 맞지 않으면 에러
			assert(false);
			break;
		}
	}
}

void TestObject::BodyPlatformCheck()
{
	// 충돌중인 모든 Platform을 찾는다.
	vector<std::pair<Collider*, Collider*>> collided;
	scene->GetCollisionMgr()->CheckCollision(m_bodyCollider, OBJECT_TYPE::PLATFORM, collided);

	// 기본적으로 모든 상황에 대비해 Falling을 켜준다.
	m_physics.isFalling = true;

	if (!collided.empty())
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
				m_physics.force.x = 0;
				isBumpWall = true;
			}

			// 땅에 닿는 순간
			if (side == SIDE::UPPER_SIDE)
			{
				m_physics.JumpReset();	// 점프 초기화
			}

			switch (side)
			{
			case SIDE::UPPER_SIDE:
				std::cout << "UPPER_SIDE" << std::endl;
				break;
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
	float resist = 0.0f;

	// 만약 잡고있는 벽이 없는 경우 모든 상황을 대비해 Resistance.y 를 0으로 초기화시킨다.
	if (collided.empty())
	{
		m_physics.resistance.y = 0;
	}
	else
	{
		// 모든 충돌체에 대해 슬라이딩이 가능한 충돌체에 HandCollider가 닿아있는지 확인
		for (auto& platform : collided)
		{
			// 하나라도 slidable인 벽이 있으면
			if (platform.second->options.slidable == true)
			{
				resist = Math::Max(resist, platform.second->options.resistance.y);

				if (m_physics.force.y > 0)
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
		m_physics.resistance.y = resist;

		if (!m_physics.isWallSliding)
			m_physics.WallSlidingStart();
		else
			m_physics.WallSlidingStay();
	}
	else if (m_physics.isWallSliding)
	{
		m_physics.WallSlidingEnd();
	}

}
