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
	
	short  jumpCount;
	short  maxJumpCount;
	
	float  jumpForce;
	
	bool   isFalling;
	double fallTime;

	bool   isMoving;

public:
	void MovingX(float _x);
	void Jump(float _jumpForce = 0);
	void JumpReset() { jumpCount = 0; }
	void JumpBlock() { jumpCount = maxJumpCount; }
	
	void CalcResistance();
	SIDE CollisionCorrect(OUT D3DXVECTOR2& correctDir, class Collider* movingCollider, class Collider* FixedCollider);
};