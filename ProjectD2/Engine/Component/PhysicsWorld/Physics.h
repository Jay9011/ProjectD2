#pragma once
class Physics
{
public:
	Physics();

public:
	GameObject* owner;

	D3DXVECTOR3 force;
	float mass;

	D3DXVECTOR3 resistance;
	D3DXVECTOR3 airResistance;
	
	float  jumpForce;
	bool   isFalling;
	double fallTime;

	bool   isMoving;

public:
	void MovingX(float _x);
	
	void CalcResistance();
};