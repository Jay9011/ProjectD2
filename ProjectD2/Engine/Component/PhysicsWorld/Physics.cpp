#include "Framework.h"
#include "Physics.h"

#include "Engine/Object/GameObject.h"

Physics::Physics() :
	owner(nullptr)
	, force({ 0, 0 })
	, mass(1)
	, resistance({ 30, 0 })
	, airResistance({ 1, 0 })
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

D3DXVECTOR2 Physics::CorrectionCollision(const FRECT& ownerRect, const FRECT& blockRect)
{
	D3DXVECTOR2 result = { 0, 0 };
	
	D3DXVECTOR2 beforeDirection = owner->GetDirection();
	
	// 움직임이 없는 경우 충돌 보정을 하지 않는다.
	if(beforeDirection == D3DXVECTOR2(0, 0))
		return result;

	
	
	D3DXVECTOR2 beforePos = owner->GetBeforePos();
	D3DXVECTOR2 nowPos    = owner->GetPos();
	D3DXVECTOR2 posDiff   = nowPos - beforePos;
	
	D3DXVECTOR2 ownerCenter       = { (ownerRect.left + ownerRect.right) * 0.5f, (ownerRect.top + ownerRect.bottom) * 0.5f };
	D3DXVECTOR2 ownerBeforeCenter = { ownerCenter.x - posDiff.x, ownerCenter.y - posDiff.y };
	D3DXVECTOR2 blockCenter       = { (blockRect.left + blockRect.right) * 0.5f, (blockRect.top + blockRect.bottom) * 0.5f };

	float ownerLower = ownerRect.top + (ownerRect.bottom - ownerRect.top) * 0.77f;
	float ownerUpper = ownerRect.top + (ownerRect.bottom - ownerRect.top) * 0.23f;


	return result;
}