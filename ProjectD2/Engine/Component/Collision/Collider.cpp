#include "Framework.h"
#include "Collider.h"

#include "Engine/Object/GameObject.h"
#include "Collision.h"
#include "Colliders/AARect.h"
#include "Colliders/Circle.h"
#include "Colliders/Line.h"

UINT Collider::m_idCounter = 0;

Collider::Collider(GameObject* _owner, int _updateOrder, string _tag) :
	Collider(_owner->GetType(), _owner, _updateOrder, _tag)
{
	
}

Collider::Collider(OBJECT_TYPE _type, GameObject* _owner, int _updateOrder, string _tag) :
	Component(_owner, _updateOrder)
    , m_owner(_owner)
    , m_type(_type)
	, isActive(false)
	, m_id(m_idCounter++)
	, m_tag(_tag)
{
	CollisionMgr* collisionMgr = _owner->GetScene()->GetCollisionMgr();
	if (collisionMgr)
	{
		collisionMgr->AddCollider(_type, this);
	}
}

Collider::~Collider()
{
    CollisionMgr* collisionMgr = m_owner->GetScene()->GetCollisionMgr();
    if (collisionMgr)
    {
        collisionMgr->RemoveCollider(m_type, this);
    }
}

/*
* �浹�� �浹 ����(Enter, Stay, Exit)�� �����Ѵ�.
*/
void Collider::OnCollision(Collider* _other)
{
	UINT otherID = _other->GetID();
	auto iter = m_collisionMap.find(otherID);

	// �������� �浹�ϰ� ���� �ʾҴ� ���
	if(iter == m_collisionMap.end())
	{
		auto result = m_collisionMap.insert(make_pair(otherID, CollisionCheck(true, true, _other)));
		iter = result.first;
		OnCollisionEnter(_other);
	}
	// ������ �浹 ������ ����Ǿ� �ִ� ���
	else
	{
		iter->second.isThisFrmChk = true;
        
		// �������� �浹�ϰ� �־��� ���
		if (iter->second.isCollision)
		{
			// �浹ü �� �� �ϳ��� ���� �����̶��
			if(GetOwner()->IsDead() || _other->GetOwner()->IsDead())
			{
				OnCollisionExit(_other);
				iter->second.isCollision = false;
			}
			else
			{
				OnCollisionStay(_other);
			}
		}
		// �������� �浹���� �ʾҴ� ��� (���� frame���� false�� �־���ϴµ�...)
		else
		{
			if (!GetOwner()->IsDead() && !_other->GetOwner()->IsDead())
			{
				OnCollisionEnter(_other);
				iter->second.isCollision = true;
			}
		}
	}
}

void Collider::FinalUpdate()
{
	// Collided �ʱ�ȭ
	isCollided = false;

	auto iter = m_collisionMap.begin();
	while (iter != m_collisionMap.end())
	{
		// �̹��� �˻����� ���� �͵鸸
		if (!iter->second.isThisFrmChk)
		{
			// ���� �����ӿ��� �浹���� ���� ������ �ǴܵǴ� ���
			if (!iter->second.isCollision)
			{
				iter = m_collisionMap.erase(iter);
				continue;
			}
            
			iter->second.isCollision = false;
			OnCollisionExit(iter->second.other);
		}
        
		iter->second.isThisFrmChk = false;	// ���� ������ üũ���θ� false�� �ʱ�ȭ
		++iter;
	}
}

void Collider::ActiveOff()
{
	// Active�� ���ԵǸ� ����
    auto iter = m_collisionMap.begin();
    while (iter != m_collisionMap.end())
    {
		if (iter->second.isCollision)	// ������ �浹�ϰ� �ִ� ��� ��ü�� ���� �浹 ������ ���ش�.
		{
			iter->second.isCollision = false;

			OnCollisionExit(iter->second.other);
			iter->second.other->OnCollisionExit(this);
		}
        
		iter->second.isThisFrmChk = false;
        ++iter;
    }
}
