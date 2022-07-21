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