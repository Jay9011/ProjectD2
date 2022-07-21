#include "Framework.h"
#include "Collider.h"

#include "Colliders/AARect.h"
#include "Colliders/Circle.h"

Collider::Collider(GameObject* _owner, int _updateOrder) :
	Component(_owner, _updateOrder)
	, isActive (false)
{
}

Collider::~Collider() = default;

bool Collider::Intersects(Collider* _other, OUT CollisionInfo& outColl)
{
	COLLIDER_TYPE otherType = _other->GetColliderType();

	switch (otherType)
	{
	case COLLIDER_TYPE::LINE:
		break;
	case COLLIDER_TYPE::AARECT:
		Intersects((AARect*)_other, outColl);
		break;
	case COLLIDER_TYPE::CIRCLE:
		Intersects((Circle*)_other, outColl);
		break;
	default:
		break;
	}
	
	return false;
}
