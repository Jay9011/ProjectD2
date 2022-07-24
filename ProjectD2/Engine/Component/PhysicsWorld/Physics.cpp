#include "Framework.h"
#include "Physics.h"

#include "Engine/Object/GameObject.h"

Physics::Physics() :
	owner(nullptr)
	, force({ 0, 0, 0 })
	, mass(1)
	, jumpForce(0)
	, isFalling(true)
	, fallTime(0)
{
}

