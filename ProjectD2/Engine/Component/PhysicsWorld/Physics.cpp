#include "Framework.h"
#include "Physics.h"

#include "Engine/Object/GameObject.h"
#include "Engine/Component/Collision/Collider.h"
#include "Engine/Component/Collision/Collision.h"

Physics::Physics() :
	owner(nullptr)
	, force({ 0.0f, 0.0f })
	, mass(1.0f)
	, speed(200.0f)
	, resistance({ 30.0f, 0.0f })
	, airResistance({ 1.0f, 0.0f })
	, jumpForce(500)
	, jumpCount(1)
	, maxJumpCount(1)
	, isFalling(true)
	, fallTime(0)
	, isWallSliding(false)
	, WallJumpDelayTime(0.1f)
{
	QueryPerformanceFrequency(&frequency);
}

void Physics::MovingX(float _x)
{
	if (isFalling)
	{
		force.x += _x * speed * fDT * 2.0f;
        
		if (force.x * force.x >= speed * speed)
			force.x = _x * speed;
	}
	else
	{
		force.x = _x * speed;
	}
}

bool Physics::Jump(float _jumpForce)
{
	QueryPerformanceCounter(&endTime);
    float timer = (float)(endTime.QuadPart - startTime.QuadPart) / (float)frequency.QuadPart;

	// 만약 벽 슬라이딩 중이었다면 반대쪽 방향으로 뛰게 한다.
	if (isWallSliding || timer <= WallJumpDelayTime)
	{
		D3DXVECTOR2 dir = { owner->GetScale().x, 0 };
		D3DXVec2Normalize(&dir, &dir);

		jumpCount = 1;

		if (_jumpForce != 0)
			force.y = -_jumpForce;
		else
			force.y = -jumpForce;

		if (isWallSliding)
			force.x = -dir.x * speed;
		else
			force.x = dir.x * speed;

		return true;
	}
    
	if (jumpCount < maxJumpCount)
	{
		++jumpCount;

		fallTime = 0;
		isFalling = true;
    
        if(_jumpForce != 0)
            force.y = -_jumpForce;
		else
			force.y = -jumpForce;
        
		return true;
	}

	return false;
}

inline void Physics::WallSlidingStart()
{
#if _DEBUG
	std::cout << "WallSliding Start" << std::endl;
#endif // _DEBUG
    
	isWallSliding = true;
}

inline void Physics::WallSlidingStay() { isWallSliding = true; }

inline void Physics::WallSlidingEnd() 
{
#if _DEBUG
	std::cout << "WallSliding End" << std::endl;
#endif // _DEBUG
    
	QueryPerformanceCounter(&startTime);
    
	isWallSliding = false;
	resistance.y = 0;
}

void Physics::CalcResistance()
{
	D3DXVECTOR2 resist = { 0, 0 };
	
	if (isFalling)
	{
		resist.x -= (force.x * airResistance.x) * fDT;
        resist.y -= (force.y * airResistance.y) * fDT;
	}
	else
	{
		resist.x -= (force.x * resistance.x) * fDT;
	}
    
	if (isWallSliding)
	{
        resist.y -= force.y * resistance.y * fDT;
	}

	// 최대 마찰력은 force 정지 마찰력으로 한다.
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

	// 상하 충돌
	if (cRect.Size().x > cRect.Size().y)
	{
		isFalling = false;

		// y가 0.3 이하면 충돌 보정 하지 않음
		bool correct = cRect.Size().y > 0.3f ? true : false;

		if (cRect.Pos().y < mRect.Pos().y)
		{
			// 아래에서 충돌
			collSide = SIDE::LOWER_SIDE;
			isFalling = true;

			if (correct)
			{
				correctDir.y = correctDir.y > cRect.Size().y ? correctDir.y : cRect.Size().y;
				if (force.y < 0)
					force.y = 0;
			}
		}
		else
		{
			// 위에서 충돌
			collSide = SIDE::UPPER_SIDE;
			if (correct)
			{
				correctDir.y = correctDir.y < -cRect.Size().y ? correctDir.y : -cRect.Size().y;
                if(force.y > 0)
                    force.y = 0;
			}
		}
	}
	// 좌우 충돌
	else
	{
		if (cRect.Pos().x < mRect.Pos().x)
		{
			// 오른쪽에서 충돌
			collSide = SIDE::RIGHT_SIDE;
			correctDir.x = correctDir.x > cRect.Size().x ? correctDir.x : cRect.Size().x;

			if(force.x < 0)
                force.x = 0;
		}
		else
		{
			// 왼쪽에서 충돌
			collSide = SIDE::LEFT_SIDE;
			correctDir.x = correctDir.x < -cRect.Size().x ? correctDir.x : -cRect.Size().x;

            if(force.x > 0)
                force.x = 0;
		}
	}

	return collSide;
}

void Physics::BodyCollision(Collider* bodyCollider)
{
	if (owner == nullptr)
		return;
    
	// 충돌중인 모든 Platform을 찾는다.
	vector<std::pair<Collider*, Collider*>> collided;
    owner->GetScene()->GetCollisionMgr()->CheckCollision(bodyCollider, OBJECT_TYPE::PLATFORM, collided);

	// 기본적으로 모든 상황에 대비해 Falling을 켜준다.
	isFalling = true;

	if (!collided.empty())
	{
		D3DXVECTOR2 correctPos = { 0, 0 };
		SIDE side = SIDE::NONE;

		// 모든 충돌체에 대한 조건 처리
		for (const auto& collider : collided)
		{
			side = CollisionCorrect(correctPos, bodyCollider, collider.second);	// 충돌 방향과 크기 체크

			// 땅에 닿는 순간
			if (side == SIDE::UPPER_SIDE)
			{
				JumpReset();	// 점프 초기화
				correctPos.y += 1.0f;
			}
		}

		// 위치 보정
		if (correctPos.x != 0 || correctPos.y != 0)
		{
            owner->AddPos(correctPos);
		}
	}
}
