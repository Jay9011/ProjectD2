#include "Framework.h"
#include "Physics.h"

#include "Engine/Object/GameObject.h"
#include "Engine/Component/Collision/Collider.h"
#include "Engine/Component/Collision/Collision.h"

Physics::Physics() :
	owner(nullptr)
	, force({ 0.0f, 0.0f })
	, mass(1.0f)
	, resistance({ 30.0f, 0.0f })
	, airResistance({ 1.0f, 0.0f })
    , jumpCount(1)
    , maxJumpCount(1)
	, jumpForce(500)
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

void Physics::Jump(float _jumpForce)
{
	if (jumpCount < maxJumpCount)
	{
		++jumpCount;

		fallTime = 0;
		isFalling = true;
    
        if(_jumpForce != 0)
            force.y = -_jumpForce;
		else
			force.y = -jumpForce;
	}
}

void Physics::CalcResistance()
{
	D3DXVECTOR2 resist;
	
	if (isFalling)
	{
		resist.x = -(force.x * airResistance.x) * fDT;
        resist.y = -(force.y * airResistance.y) * fDT;
	}
	else
	{
		resist.x = -(force.x * resistance.x) * fDT;
        resist.y = -(force.y * resistance.y) * fDT;
	}

	if (resist.x * resist.x >= force.x * force.x)
	{
		resist.x = -force.x;
	}

    if (resist.y * resist.y >= force.y * force.y)
    {
        resist.y = -force.y;
    }
	
	force += resist;
}

SIDE Physics::CollisionCorrect(OUT D3DXVECTOR2& correctDir, class Collider* movingCollider, class Collider* FixedCollider)
{
	SIDE collSide = SIDE::NONE;
	FRECT cRect = GetCollisionRect((AARect*)movingCollider, (AARect*)FixedCollider);
    FRECT mRect = movingCollider->GetRect();

	// ���� �浹
	if (cRect.Size().x > cRect.Size().y)
	{
		isFalling = false;

		// y�� 0.3 ���ϸ� �浹 ���� ���� ����
		bool correct = cRect.Size().y > 0.3f ? true : false;

		if (cRect.Pos().y < mRect.Pos().y)
		{
			// �Ʒ����� �浹
			collSide = SIDE::LOWER_SIDE;
			isFalling = true;

			if (correct)
				correctDir.y = correctDir.y > cRect.Size().y ? correctDir.y : cRect.Size().y;
		}
		else
		{
			// ������ �浹
			collSide = SIDE::UPPER_SIDE;
			if (correct)
				correctDir.y = correctDir.y < -cRect.Size().y ? correctDir.y : -cRect.Size().y;
		}
		if (correct)
			force.y = 0;
	}
	// �¿� �浹
	else
	{
		if (cRect.Pos().x < mRect.Pos().x)
		{
			// �����ʿ��� �浹
			correctDir.x = correctDir.x > cRect.Size().x ? correctDir.x : cRect.Size().x;
			collSide = SIDE::RIGHT_SIDE;
		}
		else
		{
			// ���ʿ��� �浹
			correctDir.x = correctDir.x < -cRect.Size().x ? correctDir.x : -cRect.Size().x;
			collSide = SIDE::LEFT_SIDE;
		}

		force.x = 0;
	}

	return collSide;
}
