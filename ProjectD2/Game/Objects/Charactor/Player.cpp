#include "Framework.h"
#include "Player.h"

#include "Engine/Component/Component.h"
#include "Engine/Component/Animator/Animator.h"
#include "Engine/Component/Collision/Collision.h"
#include "Engine/Component/PhysicsWorld/Physics.h"
#include "Engine/Resource/Shader.h"

#include "Game/Objects/Charactor/Part/Part.h"
#include "Game/Objects/Charactor/Projectile/BulletManager.h"
#include "Game/Objects/Monster/Monster.h"

Player::Player(Scene* _scene, int _updateOrder, GameObject* _parent) :
	Player(_scene, OBJECT_TYPE::DEFAULT, _updateOrder, _parent)
{}

Player::Player(Scene* _scene, OBJECT_TYPE _type, int _updateOrder, GameObject* _parent) :
	GameObject(_scene, _type, _updateOrder, _parent)
	, scene(_scene)
	, m_isRight(true)
	, m_preventKey(true)
    , m_isAttack(false)
	, m_equipChangeable(true)
	, m_state(PLAYER_STATE::APPEAR)
    , m_equip(PLAYER_EQUIP_TYPE::GUN)
	, m_endAttackTimerOn(false)
    , m_endAttackTime(0)
	, m_endAttackTimeMax(0.0f)
	, m_reloadTime(0)
	, m_reloadTimeMax(0.5f)
    , m_isReload(true)
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
	m_observable = scene->GetGame()->playerObservable;
    
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
	m_handCollider = ADDCOMP::NewAARect({0, -15}, {18, 0}, OBJECT_TYPE::PLAYER_ETC, this);
	m_handCollider->IsActive(true);
	m_handCollider->SetTag("hand");
    /*
	* Part
	*/
	m_handAttackPoint = new Part(_scene, _updateOrder, this);
	m_handAttackPointOrigin = { 18, -8 };
	m_handAttackPoint->SetPos(m_handAttackPointOrigin);
	m_swordOriginMin = { -15, -30 };
	m_swordOriginMax = { 60, 30 };
	
	/* === === === === ===
	*   Init Settings
	* === === === === === */
	m_status.maxHp = 10.0f;
	m_status.hp = 10.0f;
	m_observable->Notify(*this, "GetMaxHP");
	m_observable->Notify(*this, "GetHP");
	/*
	* Physics
	*/
	m_physics.owner = this;
	m_physics.maxJumpCount = 2;
	m_physics.mass = 1.5;
	m_physics.speed = 300.0f;
	m_physics.jumpForce = 400.f;
	ADDCOMP::NewGravity(m_physics, this);
	/*
	* Bullets
	*/
	m_bulletManager = new BulletManager(30, 1000.0f, 1.0f, 500.0f, _scene, _updateOrder);
	m_bulletManager->SetScale({ 2.0f, 2.0f });
	/*
	* Sword
	*/
	m_atkInfo.type = ATK_Type::SWORD;
	m_atkInfo.damage = 5.0f;
	m_swordCollider = ADDCOMP::NewAARect(m_swordOriginMin, m_swordOriginMax, OBJECT_TYPE::PLAYER_ATK, this);
	m_swordCollider->SetCallbackOnCollisionEnter([this](Collider* _other) {
		if (_other->GetOwner()->GetType() == OBJECT_TYPE::MONSTER)
		{
            Monster* monster = dynamic_cast<Monster*>(_other->GetOwner());
			if(monster == nullptr)
                return;
            
			D3DXVECTOR2 dir = GetWorldPos() - monster->GetWorldPos();
			monster->OnHit(m_atkInfo, dir);
		}
	});
    /*
	* 
	*/
	SetAnimation();
	m_animator->Find((int)m_state)->Play();
}

Player::~Player()
{
    delete m_bulletManager;
}

void Player::UpdateObject()
{
	Move();
	Attack();
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

void Player::Damage(float _damage)
{
    if(m_state == PLAYER_STATE::CRITICAL || m_state == PLAYER_STATE::DIE)
        return;

	CAMERA->Shake(0.2f, 100.0f);
    m_status.hp -= _damage;
	m_observable->Notify(*this, "GetHP");
	UpdateState(PLAYER_STATE::HIT, m_equip);
    
    if (m_status.hp <= 0 && !m_isDie)
    {
		m_isDie = true;
		m_preventKey = true;
        m_status.hp = 0;
		m_state = PLAYER_STATE::IDLE;
		m_isAttack = false;
		UpdateState(PLAYER_STATE::CRITICAL, m_equip);
    }
}

void Player::Die()
{
    UpdateState(PLAYER_STATE::DIE, m_equip);
}

void Player::DieEnd()
{
	m_bodyCollider->IsActive(false);
	SetState(OBJECT_STATE::DEAD);
}

void Player::Move()
{
    if (m_preventKey || m_isDie)
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

void Player::Attack()
{
	if (m_isDie)
		return;
    
	if (m_reloadTime < m_reloadTimeMax)
		m_reloadTime += fDT;

	if (m_preventKey)
		return;
    
	if (KEYPRESS('D'))
	{
		if (m_reloadTime < m_reloadTimeMax)
			return;
		else
            m_reloadTime = 0.0f;

		if (m_equip == PLAYER_EQUIP_TYPE::GUN)
		{
			D3DXVECTOR2 dir = m_isRight ? V_RIGHT : V_LEFT;
			m_bulletManager->Fire(m_handAttackPoint->GetWorldPos(), dir * m_iAttackReverse);
		}
		else
		{
			// ���� ����
			m_swordCollider->IsActive(true);
		}
        
		/*
		* ���º� ���� ���� ��ȯ
		*/
		switch (m_state)
		{
		case PLAYER_STATE::HIT:
		case PLAYER_STATE::CRITICAL:
		case PLAYER_STATE::LANDING:
		case PLAYER_STATE::IDLE:
			UpdateState(PLAYER_STATE::IDLE_ATK, m_equip);
			break;
		case PLAYER_STATE::RUN:
			UpdateState(PLAYER_STATE::RUN_ATK, m_equip);
			break;
		case PLAYER_STATE::FALL:
		case PLAYER_STATE::JUMP:
			UpdateState(PLAYER_STATE::JUMP_ATK, m_equip);
			break;
		case PLAYER_STATE::HANG:
			UpdateState(PLAYER_STATE::HANG_ATK, m_equip);
			break;
		default:
			break;
		}

		m_isAttack = true;
		m_equipChangeable = false;	// ���� ����߿��� ���⸦ �ٲ��� ���Ѵ�.
        
		UpdateState(m_prevState, m_equip);		// ���� ���¸� �����صд�.
	}
	
}

void Player::AttackEnd()
{
	m_swordCollider->IsActive(false);
	m_isAttack = false;
	m_equipChangeable = true;	// ���ݸ���� ������ ���� ���� ����
    
	if (m_reservState != PLAYER_STATE::APPEAR)	// Appear ���¸� ������ ���� ���·� ���...
	{
		UpdateState(m_reservState, m_equip);
		return;
	}
    
	//UpdateState(PLAYER_STATE::IDLE, m_equip);	// �ƹ� ������ ���ٸ� IDLE�� �����Ѵ�.
}

void Player::ChangeWeapon()
{
	if (m_preventKey || !m_equipChangeable || m_isDie)
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
		
		m_observable->Notify(*this, "GetEquip");
	}
}

void Player::StateProcessing()
{
    if (m_isDie)
        return;
    
	// Idle ���� üũ
	if (m_state == PLAYER_STATE::RUN &&(KEYNONE(VK_LEFT) && KEYNONE(VK_RIGHT))	// �޸��� �����ε�, �̵� Ű�� ������ �ʰ��ִٸ� Idle ���·� �����Ѵ�.
		)
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
    if (m_isDie)
        return;
    
	if (m_state == PLAYER_STATE::HANG || m_state == PLAYER_STATE::HANG_ATK)
	{
		m_iAttackReverse = -1;
		D3DXVECTOR2 handPos = m_handAttackPoint->GetPos();
		if (handPos == m_handAttackPointOrigin)
		{
			m_handAttackPoint->SetPos(handPos.x * -1, handPos.y);
		}
		D3DXVECTOR2 swordMin = m_swordCollider->GetLocalMin();
		if (swordMin == m_swordOriginMin)
		{
			D3DXVECTOR2 swordMax = m_swordCollider->GetLocalMax();
			m_swordCollider->SetMin({ swordMin.x * -1, swordMin.y });
			m_swordCollider->SetMax({ swordMax.x * -1, swordMax.y });
		}
	}
	else
	{
		m_iAttackReverse = 1;
		D3DXVECTOR2 handPos = m_handAttackPoint->GetPos();
		if (handPos != m_handAttackPointOrigin)
		{
			m_handAttackPoint->SetPos(m_handAttackPointOrigin);
		}
		D3DXVECTOR2 swordMin = m_swordCollider->GetLocalMin();
		if (swordMin != m_swordOriginMin)
		{
			m_swordCollider->SetMin(m_swordOriginMin);
			m_swordCollider->SetMax(m_swordOriginMax);
		}
	}

	if (m_endAttackTimerOn)
	{
		m_endAttackTime -= fDT;
        if (m_endAttackTime <= 0.0f)
        {
            m_endAttackTime = 0.0f;
			m_endAttackTimerOn = false;
            AttackEnd();
        }
	}
}

void Player::UpdateState(PLAYER_STATE _state, PLAYER_EQUIP_TYPE _equip)
{
	if (m_state == _state && m_equip == _equip)
		return;
    
	/*
	* ���� ���ݸ�����̾��ٸ� ���¸� �����ϰ� �����Ѵ�.
	*/
	if(m_isAttack)
    {
		if (_state == PLAYER_STATE::HIT || _state == PLAYER_STATE::CRITICAL)	// ���� �� ��Ʈ �ִϸ��̼��� ������� �ʴ´�.
			return;

        m_reservState = _state;
		return;
    }
    
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
        m_animator->Find((int)PLAYER_ANIM::DIE)->Play();
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
		if (m_prevState == PLAYER_STATE::RUN_ATK && m_equip == PLAYER_EQUIP_TYPE::GUN)
            break;
            
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
			m_endAttackTime = m_endAttackTimeMax;
			m_endAttackTimerOn = true;
            //m_animator->Find((int)PLAYER_ANIM::RUN_ATK_GUN)->Play();
        }
        if(m_equip == PLAYER_EQUIP_TYPE::SWORD)
        {
            m_animator->Find((int)PLAYER_ANIM::RUN_ATK_SWD)->Play();
        }
		break;
	case PLAYER_STATE::JUMP:
	{
		if (m_prevState == PLAYER_STATE::JUMP_ATK && m_equip == PLAYER_EQUIP_TYPE::GUN)
			break;
        
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
			m_endAttackTime = m_endAttackTimeMax;
			m_endAttackTimerOn = true;
            //m_animator->Find((int)PLAYER_ANIM::JUMP_ATK_GUN)->Play();
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
		else if (m_prevState == PLAYER_STATE::HANG_ATK)
		{
			m_animator->Find((int)PLAYER_ANIM::HANG)->Play(ANIM_PLAY_FLAG::SetFrameToEnd, 3, -1);	// ���� ���°� �Ŵ޸� ���¿��ٸ� ������ �κи� ����Ѵ�.
			break;
		}
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
				});
			break;
		case PLAYER_ANIM::HIT:
			m_animator->LoadXML("Character\\Player\\", "Hit", ANIM_PLAY_TYPE::ONCE, 0.05f);
			m_animator->SetEndEvent(i, [this]() {
				UpdateState(PLAYER_STATE::IDLE, m_equip);
				});
			break;
		case PLAYER_ANIM::CRITICAL:
			m_animator->LoadXML("Character\\Player\\", "Critial", ANIM_PLAY_TYPE::ONCE, 0.05f);
			m_animator->SetEndEvent(i, [this]() {
				Die();
				});
			break;
		case PLAYER_ANIM::DIE:
			m_animator->LoadXML("SFX\\Explosion\\", "Effect03", ANIM_PLAY_TYPE::ONCE, 0.1f);
			m_animator->SetEndEvent(i, [this]() {
				DieEnd();
				});
			break;
		case PLAYER_ANIM::IDLE:
			m_animator->LoadXML("Character\\Player\\", "Idle", ANIM_PLAY_TYPE::PINGPONG, 0.2f);
			break;
		case PLAYER_ANIM::IDLE_ATK_GUN:
			m_animator->LoadXML("Character\\Player\\GUN\\", "IdleAttack", ANIM_PLAY_TYPE::ONCE, 0.05f);
			m_animator->SetEndEvent(i, [this]() {
				AttackEnd();
                });
			break;
		case PLAYER_ANIM::IDLE_ATK_SWD:
			m_animator->LoadXML("Character\\Player\\SWD\\", "IdleAttack", ANIM_PLAY_TYPE::ONCE, 0.03f);
			m_animator->SetEndEvent(i, [this]() {
				AttackEnd();
				});
			break;
		case PLAYER_ANIM::RUN_GUN:
			m_animator->LoadXML("Character\\Player\\GUN\\", "Run", ANIM_PLAY_TYPE::LOOP, 0.05f);
			break;
		case PLAYER_ANIM::RUN_SWD:
			m_animator->LoadXML("Character\\Player\\SWD\\", "Run", ANIM_PLAY_TYPE::LOOP, 0.05f);
			break;
		case PLAYER_ANIM::RUN_ATK_GUN:
			m_animator->LoadXML("Character\\Player\\GUN\\", "RunAttack", ANIM_PLAY_TYPE::LOOP, 0.05f);
			break;
		case PLAYER_ANIM::RUN_ATK_SWD:
			m_animator->LoadXML("Character\\Player\\SWD\\", "IdleAttack", ANIM_PLAY_TYPE::LOOP, 0.03f);
			m_animator->SetEndEvent(i, [this]() {
				AttackEnd();
				});
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
			m_animator->LoadXML("Character\\Player\\SWD\\", "JumpAttack", ANIM_PLAY_TYPE::ONCE, 0.03f);
			m_animator->SetEndEvent(i, [this]() {
				AttackEnd();
				});
			break;
		case PLAYER_ANIM::HANG:
			m_animator->LoadXML("Character\\Player\\", "HoldOn", ANIM_PLAY_TYPE::ONCE, 0.05f);
			break;
		case PLAYER_ANIM::HANG_ATK_GUN:
			m_animator->LoadXML("Character\\Player\\GUN\\", "HoldOnAttack", ANIM_PLAY_TYPE::ONCE, 0.05f);
			m_animator->SetEndEvent(i, [this]() {
				AttackEnd();
				});
			break;
		case PLAYER_ANIM::HANG_ATK_SWD:
			m_animator->LoadXML("Character\\Player\\SWD\\", "HoldOnAttack", ANIM_PLAY_TYPE::ONCE, 0.03f);
			m_animator->SetEndEvent(i, [this]() {
				AttackEnd();
				});
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
	if (m_isDie)
		return;
    
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
		if (m_state == PLAYER_STATE::HANG || m_state == PLAYER_STATE::HANG_ATK)
		{
			UpdateState(PLAYER_STATE::IDLE, m_equip);
		}
	}

}
