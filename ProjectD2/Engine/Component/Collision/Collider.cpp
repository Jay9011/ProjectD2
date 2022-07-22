#include "Framework.h"
#include "Collider.h"

#include "Engine/Object/GameObject.h"
#include "Collision.h"
#include "Colliders/AARect.h"
#include "Colliders/Circle.h"
#include "Colliders/Line.h"

Collider::Collider(GameObject* _owner, int _updateOrder) :
	Component(_owner, _updateOrder)
	, isActive (false)
{
	CollisionMgr* collisionMgr = _owner->GetScene()->GetCollisionMgr();
	if (collisionMgr)
	{
		collisionMgr->AddCollider(_owner->GetType(), this);
	}
}

Collider::~Collider() = default;