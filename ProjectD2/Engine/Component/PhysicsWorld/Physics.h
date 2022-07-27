#pragma once
class Physics
{
public:
	Physics();

public:
	GameObject* owner;

	D3DXVECTOR2 force;
	float mass;

	D3DXVECTOR2 resistance;
	D3DXVECTOR2 airResistance;
	
	float  jumpForce;
	
	short  jumpCount;
	short  maxJumpCount;
	
	bool   isFalling;
	double fallTime;

	bool   isWallSliding;
    
public:
	void MovingX(float _x);
	void Jump(float _jumpForce = 0);
	void JumpReset() { jumpCount = 0; }
	void JumpBlock() { jumpCount = maxJumpCount; }

	void WallSlidingStart();
	void WallSlidingStay();
	void WallSlidingEnd();
	
	void CalcResistance();
	SIDE CollisionCorrect(OUT D3DXVECTOR2& correctDir, class Collider* movingCollider, class Collider* FixedCollider);
};