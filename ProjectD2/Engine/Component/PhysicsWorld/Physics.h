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
	bool   isFalling;
	double fallTime;

	bool   isMoving;

public:
	void MovingX(float _x);
	
	void CalcResistance();

	D3DXVECTOR2 CorrectionCollision(const FRECT& ownerRect, const FRECT& blockRect);
};