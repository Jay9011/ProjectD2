#pragma once
class Physics
{
public:
	Physics();

public:
	GameObject* owner;

	D3DXVECTOR2 force;
	float mass;
	float speed;

	D3DXVECTOR2 resistance;
	D3DXVECTOR2 airResistance;
	
	float  jumpForce;
	
	short  jumpCount;
	short  maxJumpCount;
	
	bool   isFalling;
	double fallTime;

	bool   isWallSliding;
    
private:
	double WallJumpDelayTime;
    
	LARGE_INTEGER startTime{};
	LARGE_INTEGER endTime{};
	LARGE_INTEGER frequency{};
    
public:
	void MovingX(float _x);
	bool Jump(float _jumpForce = 0);
	void JumpReset() { jumpCount = 0; }
	void JumpBlock() { jumpCount = maxJumpCount; }

	void WallSlidingStart();
	void WallSlidingStay();
	void WallSlidingEnd();
	
	void CalcResistance();
	SIDE CollisionCorrect(OUT D3DXVECTOR2& correctDir, class Collider* movingCollider, class Collider* FixedCollider);

	void BodyCollision(class Collider* bodyCollider);
};