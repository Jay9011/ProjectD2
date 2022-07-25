#include "Framework.h"
#include "Physics.h"

#include "Engine/Object/GameObject.h"

Physics::Physics() :
	owner(nullptr)
	, force({ 0, 0, 0 })
	, mass(1)
	, resistance({ 10, 0, 0 })
	, airResistance({ 1, 0, 0 })
	, jumpForce(0)
	, isFalling(true)
	, fallTime(0)
{
}

void Physics::MovingX(float _x)
{
	if (isFalling)
		force.x += _x * fDT;
	else
		force.x = _x;
}

void Physics::CalcResistance()
{
	float resist;
	
	if (isFalling)
	{
		resist = -(force.x * airResistance.x) * fDT;
	}
	else
	{
		resist = -(force.x * resistance.x) * fDT;
	}

	if (resist * resist >= force.x * force.x)
	{
		resist = -force.x;
	}
	
	force.x += resist;
	
	if(Math::NearZero(force.x, 10.0f))
		force.x = 0;
}
