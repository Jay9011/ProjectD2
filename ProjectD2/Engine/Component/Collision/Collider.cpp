#include "Framework.h"
#include "Collider.h"

#include "Engine/Object/GameObject.h"
#include "Collision.h"
#include "Colliders/AARect.h"
#include "Colliders/Circle.h"
#include "Colliders/Line.h"

UINT Collider::m_idCounter = 0;

Collider::Collider(GameObject* _owner, int _updateOrder, string _tag) :
	Component(_owner, _updateOrder)
	, isActive(false)
	, m_id(m_idCounter++)
    , m_tag(_tag)
{
	CollisionMgr* collisionMgr = _owner->GetScene()->GetCollisionMgr();
	if (collisionMgr)
	{
		collisionMgr->AddCollider(_owner->GetType(), this);
	}
}

Collider::~Collider() = default;

/*
* �浹�� �浹 ����(Enter, Stay, Exit)�� �����Ѵ�.
*/
void Collider::OnCollision(Collider* _other)
{
	UINT otherID = _other->GetID();
	auto iter = m_collisionMap.find(otherID);

#if _DEBUG
	string str;
#endif // _DEBUG

	// �������� �浹�ϰ� ���� �ʾҴ� ���
	if(iter == m_collisionMap.end())
	{
		auto result = m_collisionMap.insert(make_pair(otherID, CollisionCheck(true, true, _other)));
		iter = result.first;
		OnCollisionEnter(_other);
#if _DEBUG
		str = "Collision::Enter : " + m_tag;
		std::cout << str << std::endl;
#endif // _DEBUG

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
#if _DEBUG
				str = "Collision::Exit : " + m_tag;
				std::cout << str << std::endl;
#endif // _DEBUG
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
#if _DEBUG
				str = "Collision::Enter : " + m_tag;
				std::cout << str << std::endl;
#endif // _DEBUG
			}
		}
	}
}

void Collider::FinalUpdate()
{
#if _DEBUG
	string str;
#endif // _DEBUG

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
#if _DEBUG
			str = "Collision::Exit : " + m_tag;
			std::cout << str << std::endl;
#endif // _DEBUG
		}
        
		iter->second.isThisFrmChk = false;	// ���� ������ üũ���θ� false�� �ʱ�ȭ
		++iter;
	}
}
