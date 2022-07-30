#include "Framework.h"
#include "Player.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Component/Collision/Collision.h"
#include "Engine/Component/PhysicsWorld/Physics.h"
#include "Engine/Resource/Shader.h"

Player::Player(Scene* _scene, int _updateOrder, GameObject* _parent) :
	Player(_scene, OBJECT_TYPE::DEFAULT, _updateOrder, _parent)
{}

Player::Player(Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, _type, _updateOrder, _parent)
	, scene(_scene)
	, m_isRight(true)
	, m_preventKey(true)
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

Player::~Player() = default;

void Player::UpdateObject()
{
	// ���� Ȥ�� Animation�� �����ϱ� �� ���� ���¸� �����Ѵ�.
    m_prevEquip = m_equip;
	m_prevState = m_state;
    //////////////////////////////
	
	Move();
	ChangeWeapon();
	StateProcessing();
	AnimationProcessing();
    
	// ���� ��ġ ��ȯ
	AddPos(m_physics.force * fDT);

#if _DEBUG
	m_dir = { GetDirection().x, GetDirection().y, 0 };
#endif // _DEBUG
}

void Player::RenderObject()
{
	GameObject::RenderObject();
}

void Player::PostUpdateObject()
{
	BodyPlatformCheck();
	HandPlatformCheck();

}

void Player::FinalUpdateObject()
{
	/*
	* ������ ó��
	*/
	m_physics.CalcResistance();
}

void Player::Move()
{
    if (m_preventKey)
        return;
    
	// Scale�� ���� ������ ������ �޶����� ������ Scale ���� ���� ������ ó���Ѵ�.
	if (KEYDOWN(VK_UP))
	{
		m_physics.Jump();
		m_prevState = m_state;
        m_state = PLAYER_STATE::FALL;	// �ӽ÷� �������� ���·� �����Ѵ�.
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
}

void Player::ChangeWeapon()
{
	if (m_preventKey)
		return;
    
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

void Player::StateProcessing()
{
	// Idle ���� üũ
	if (m_state == PLAYER_STATE::RUN && (KEYUP(VK_LEFT) || KEYUP(VK_RIGHT)) && (!KEYPRESS(VK_LEFT) && !KEYPRESS(VK_RIGHT)))
	{
		UpdateState(PLAYER_STATE::IDLE, m_equip);
	}
    
	// Falling ���� üũ
    if(m_physics.isFalling && !m_physics.isWallSliding && m_physics.force.y > 0 && m_state != PLAYER_STATE::APPEAR)
    {
        UpdateState(PLAYER_STATE::FALL, m_equip);
    }
}

void Player::AnimationProcessing()
{


}

void Player::UpdateState(PLAYER_STATE _state, PLAYER_EQUIP_TYPE _equip)
{
	if (m_state == _state && m_equip == _equip)
		return;
    
	m_prevState = m_state;
	m_state = _state;
	m_prevEquip = m_equip;
	m_equip = _equip;

	UpdateAnimation();
}

void Player::UpdateAnimation()
{
	if(m_prevState == m_state && m_prevEquip == m_equip)
        return;
    
	switch (m_state)
	{
	case PLAYER_STATE::APPEAR:
		if (m_prevState == PLAYER_STATE::APPEAR)	// ���� ����, �ٸ� ���� ����� ��� �ִϸ��̼� ������ ó������ �ʴ´�.
			break;									// ���� �ʿ��� ���, �ӽ÷� ���¸� ���� �� ó���ϵ��� �Ѵ�.
		m_animator->Find((int)PLAYER_ANIM::APPEAR)->Play();
		break;
	case PLAYER_STATE::HIT:
		if (m_prevState == PLAYER_STATE::HIT)
			break;
        m_animator->Find((int)PLAYER_ANIM::HIT)->Play();
		break;
	case PLAYER_STATE::CRITICAL:
		if (m_prevState == PLAYER_STATE::CRITICAL)
			break;
        m_animator->Find((int)PLAYER_ANIM::CRITICAL)->Play();
		break;
	case PLAYER_STATE::DIE:
		if (m_prevState == PLAYER_STATE::DIE)
			break;
        m_animator->Find((int)PLAYER_ANIM::CRITICAL)->Play();
		break;
	case PLAYER_STATE::IDLE:
		if (m_prevState == PLAYER_STATE::IDLE)
			break;
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
		if (m_prevState == m_state)
		{
			bool reversing = m_animator->GetCurrentAnimation()->IsReverse();
			UINT nowFrame = m_animator->GetCurrentAnimation()->GetCurrentFrame();
			if (m_equip == PLAYER_EQUIP_TYPE::GUN)
			{
				m_animator->Find((int)PLAYER_ANIM::JUMP_GUN)->Play(ANIM_PLAY_FLAG::SetFrameToEnd, nowFrame, -1, reversing);
			}
			else
			{
				m_animator->Find((int)PLAYER_ANIM::JUMP_SWD)->Play(ANIM_PLAY_FLAG::SetFrameToEnd, nowFrame, -1, reversing);
			}
		}
		else
		{
			if (m_equip == PLAYER_EQUIP_TYPE::GUN)
			{
				m_animator->Find((int)PLAYER_ANIM::JUMP_GUN)->Play();
			}
			else
			{
				m_animator->Find((int)PLAYER_ANIM::JUMP_SWD)->Play();
			}
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
	case PLAYER_STATE::FALL:
	{
		if (m_prevState == m_state)
		{
			bool reversing = m_animator->GetCurrentAnimation()->IsReverse();
			UINT nowFrame = m_animator->GetCurrentAnimation()->GetCurrentFrame();
			if (m_equip == PLAYER_EQUIP_TYPE::GUN)
			{
				m_animator->Find((int)PLAYER_ANIM::FALL_GUN)->Play(ANIM_PLAY_FLAG::SetFrameToEnd, nowFrame, -1, reversing);
			}
			else
			{
				m_animator->Find((int)PLAYER_ANIM::FALL_SWD)->Play(ANIM_PLAY_FLAG::SetFrameToEnd, nowFrame, -1, reversing);
			}
		}
		else
		{
			if (m_equip == PLAYER_EQUIP_TYPE::GUN)
			{
				m_animator->Find((int)PLAYER_ANIM::FALL_GUN)->Play();
			}
			else
			{
				m_animator->Find((int)PLAYER_ANIM::FALL_SWD)->Play();
			}
		}
	}
		break;
	case PLAYER_STATE::LANDING:
	{
		if (m_prevState == m_state)
		{
			bool reversing = m_animator->GetCurrentAnimation()->IsReverse();
			UINT nowFrame = m_animator->GetCurrentAnimation()->GetCurrentFrame();
			if (m_equip == PLAYER_EQUIP_TYPE::GUN)
			{
				m_animator->Find((int)PLAYER_ANIM::LANDING_GUN)->Play(ANIM_PLAY_FLAG::SetFrameToEnd, nowFrame, -1, reversing);
			}
			else
			{
				m_animator->Find((int)PLAYER_ANIM::LANDING_SWD)->Play(ANIM_PLAY_FLAG::SetFrameToEnd, nowFrame, -1, reversing);
			}
		}
		else
		{
			if (m_equip == PLAYER_EQUIP_TYPE::GUN)
			{
				m_animator->Find((int)PLAYER_ANIM::LANDING_GUN)->Play();
			}
			else
			{
				m_animator->Find((int)PLAYER_ANIM::LANDING_SWD)->Play();
			}
		}
	}
		break;
	case PLAYER_STATE::HANG:
		if (m_prevState == PLAYER_STATE::HANG)
			break;
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

void Player::SetAnimation()
{
	for (int i = 0; i < (int)PLAYER_ANIM::FIN; i++)
	{
		switch ((PLAYER_ANIM)i)
		{
		case PLAYER_ANIM::APPEAR:
			m_animator->LoadXML("Character\\Player\\", "Appear", ANIM_PLAY_TYPE::ONCE, 0.1f);
			m_animator->Find(i)->SetFrameDuration(16, 0.5f);
			m_animator->SetEndEvent(i, [this]() {
				UpdateState(PLAYER_STATE::IDLE, m_equip);
				m_preventKey = false;
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
			m_animator->LoadXML("Character\\Player\\GUN\\", "Jump1", ANIM_PLAY_TYPE::ONCE, 0.07f);
			break;
		case PLAYER_ANIM::FALL_GUN:
			m_animator->LoadXML("Character\\Player\\GUN\\", "Jump2", ANIM_PLAY_TYPE::ONCE, 0.05f);
			break;
		case PLAYER_ANIM::LANDING_GUN:
			m_animator->LoadXML("Character\\Player\\GUN\\", "Jump3", ANIM_PLAY_TYPE::ONCE, 0.05f);
			m_animator->SetEndEvent(i, [this]() {
				UpdateState(PLAYER_STATE::IDLE, m_equip);
				});
			break;
		case PLAYER_ANIM::JUMP_SWD:
			m_animator->LoadXML("Character\\Player\\SWD\\", "Jump1", ANIM_PLAY_TYPE::ONCE, 0.07f);
			break;
		case PLAYER_ANIM::FALL_SWD:
			m_animator->LoadXML("Character\\Player\\SWD\\", "Jump2", ANIM_PLAY_TYPE::ONCE, 0.05f);
			break;
		case PLAYER_ANIM::LANDING_SWD:
			m_animator->LoadXML("Character\\Player\\SWD\\", "Jump3", ANIM_PLAY_TYPE::ONCE, 0.05f);
			m_animator->SetEndEvent(i, [this]() {
				UpdateState(PLAYER_STATE::IDLE, m_equip);
				});
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
			// Animation ��� ������ ���� ������ ����
			assert(false);
			break;
		}
	}
}

void Player::BodyPlatformCheck()
{
	// �浹���� ��� Platform�� ã�´�.
	vector<std::pair<Collider*, Collider*>> collided;
	scene->GetCollisionMgr()->CheckCollision(m_bodyCollider, OBJECT_TYPE::PLATFORM, collided);

	// �⺻������ ��� ��Ȳ�� ����� Falling�� ���ش�.
	m_physics.isFalling = true;

	if (!collided.empty())
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
				isBumpWall = true;
			}

			// ���� ��� ����
			if (side == SIDE::UPPER_SIDE)
			{
				m_physics.JumpReset();	// ���� �ʱ�ȭ
                
                if(m_state == PLAYER_STATE::FALL)
					UpdateState(PLAYER_STATE::LANDING, m_equip);
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

void Player::HandPlatformCheck()
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

				if (m_physics.force.y > 0)
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
		{
			m_physics.WallSlidingStart();
			UpdateState(PLAYER_STATE::HANG, m_equip);
		}
		else
		{
			m_physics.WallSlidingStay();
		}
	}
	else if (m_physics.isWallSliding)
	{
		m_physics.WallSlidingEnd();
		UpdateState(PLAYER_STATE::IDLE, m_equip);
	}

}
