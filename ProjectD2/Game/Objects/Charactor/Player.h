#pragma once
#include "Engine/Object/GameObject.h"
#include "Engine/Observer/Observer.h"

class Scene;
using PlayerStatus = struct tagPlayerStatus
{
	float maxHp;
	float hp;
};
class Player : public GameObject
{
public:
	Player(Scene* _scene, int _updateOrder = 100, GameObject* _parent = nullptr);
	Player(Scene* _scene, OBJECT_TYPE _type, int _updateOrder = 100, GameObject* _parent = nullptr);
	virtual ~Player();

	// GameObject을(를) 통해 상속됨
	virtual void UpdateObject() override;
	virtual void RenderObject() override;
	virtual void PostUpdateObject() override;
	virtual void FinalUpdateObject() override;

	void Damage(float _damage);

private:
	void Move();
	void Die();
	void DieEnd();
	void Attack();
	void AttackEnd();
	void ChangeWeapon();
	void StateProcessing();
	void AnimationProcessing();

	void UpdateState(PLAYER_STATE _state, PLAYER_EQUIP_TYPE _equip);
	void UpdateAnimation();
	void SetAnimation();
	void BodyPlatformCheck();
	void HandPlatformCheck();
    
private:
	Scene* scene;

	bool  m_isRight;
	short m_iAttackReverse;
    
	bool m_preventKey;	// 키 입력 막기
	bool m_isAttack;	// 공격중인지 확인
	bool m_equipChangeable;	// 무기 변경 막기
    
	PLAYER_STATE m_prevState;
	PLAYER_STATE m_state;
	PLAYER_STATE m_reservState;
	PLAYER_EQUIP_TYPE m_prevEquip;
	PLAYER_EQUIP_TYPE m_equip;

	bool  m_endAttackTimerOn;
	float m_endAttackTime;
	float m_endAttackTimeMax;
	
	Animator* m_animator;
	
	Collider* m_bodyCollider;
	Collider* m_handCollider;
    
    AARect*     m_swordCollider;
	D3DXVECTOR2 m_swordOriginMin;
    D3DXVECTOR2 m_swordOriginMax;
	Part*	    m_handAttackPoint;
	D3DXVECTOR2 m_handAttackPointOrigin;

	/*
	* 능력치 관련
	*/
	PlayerStatus m_status;
	ATK_Info m_atkInfo;
	Physics m_physics;
    BulletManager* m_bulletManager;
    float m_reloadTime;
	float m_reloadTimeMax;
    bool  m_isReload;

    /*
	* Observer
	*/
    Observable<Player>* m_observable;

#if _DEBUG
	D3DXVECTOR3 m_dir;
#endif

private:
	void ScaleXInverse()
	{
		D3DXVECTOR2 scale = GetScale();
		scale.x *= -1;
		SetScale(scale);
	};

/* === === === === ===
*  Getter / Setter
* === === === === === */
public:
    void AddForce(D3DXVECTOR2 _force) 
	{
		m_physics.force += _force;
	}
        
    PLAYER_EQUIP_TYPE GetEquip() const { return m_equip; }
	float GetMaxHP() const { return m_status.maxHp; }
    float GetHP() const { return m_status.hp; }
    
	void SetPreventKey(bool _b) { m_preventKey = _b; }
	
	void SetWeapon(PLAYER_EQUIP_TYPE _equip) { m_equip = _equip; }
	void SwitchingWeapon() 
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
};