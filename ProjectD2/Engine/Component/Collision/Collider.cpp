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
* 충돌시 충돌 정보(Enter, Stay, Exit)를 갱신한다.
*/
void Collider::OnCollision(Collider* _other)
{
	UINT otherID = _other->GetID();
	auto iter = m_collisionMap.find(otherID);

	// 이전에는 충돌하고 있지 않았던 경우
	if(iter == m_collisionMap.end())
	{
		auto result = m_collisionMap.insert(make_pair(otherID, CollisionCheck(true, true, _other)));
		iter = result.first;
		OnCollisionEnter(_other);
	}
	// 이전에 충돌 정보가 저장되어 있던 경우
	else
	{
		iter->second.isThisFrmChk = true;
        
		// 이전에도 충돌하고 있었던 경우
		if (iter->second.isCollision)
		{
			// 충돌체 둘 중 하나가 삭제 예정이라면
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
		// 이전에는 충돌하지 않았던 경우 (이전 frame에서 false가 있어야하는데...)
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
	// Collided 초기화
	isCollided = false;

	auto iter = m_collisionMap.begin();
	while (iter != m_collisionMap.end())
	{
		// 이번에 검사하지 않은 것들만
		if (!iter->second.isThisFrmChk)
		{
			// 이전 프레임에서 충돌하지 않은 것으로 판단되는 경우
			if (!iter->second.isCollision)
			{
				iter = m_collisionMap.erase(iter);
				continue;
			}
            
			iter->second.isCollision = false;
			OnCollisionExit(iter->second.other);
		}
        
		iter->second.isThisFrmChk = false;	// 다음 프레임 체크여부를 false로 초기화
		++iter;
	}
}

void Collider::ActiveOff()
{
	// Active를 끄게되면 동작
    auto iter = m_collisionMap.begin();
    while (iter != m_collisionMap.end())
    {
		if (iter->second.isCollision)	// 이전에 충돌하고 있던 모든 물체에 대해 충돌 해제를 해준다.
		{
			iter->second.isCollision = false;

			OnCollisionExit(iter->second.other);
			iter->second.other->OnCollisionExit(this);
		}
        
		iter->second.isThisFrmChk = false;
        ++iter;
    }
}
