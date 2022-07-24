#include "Framework.h"
#include "Physics.h"

Physics::Physics() :
	Physics(0)
{
}

Physics::Physics(float jumpForce) :
	mass(1)
	, jumpForce(jumpForce)
	, isFalling(true)
	, fallTime(0)
{
}
