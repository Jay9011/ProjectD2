#include "Framework.h"
#include "Physics.h"

#include "Engine/Object/GameObject.h"

Physics::Physics() :
	owner(nullptr)
	, force({ 0.0f, 0.0f })
	, mass(1.0f)
	, resistance({ 30.0f, 0.0f })
	, airResistance({ 1.0f, 0.0f })
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
}