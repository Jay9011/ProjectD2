#include "Framework.h"
#include "Collider.h"

#include "Colliders/AARect.h"

Collider::Collider(GameObject* _owner, int _updateOrder) :
	Component(_owner, _updateOrder)
{
}

Collider::~Collider()
{
}

bool Collider::Intersects(Collider* _other)
{
	COLLIDER_TYPE otherType = _other->GetColliderType();

	switch (otherType)
	{
	case COLLIDER_TYPE::LINE:
		break;
	case COLLIDER_TYPE::AARECT:
		Intersects((AARect*)_other);
		break;
	case COLLIDER_TYPE::CIRCLE:
		break;
	default:
		break;
	}
	
	return false;
}
