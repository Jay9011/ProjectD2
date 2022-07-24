#pragma once
class Physics
{
public:
	Physics();

public:
	GameObject* owner;

	D3DXVECTOR3 force;
	float mass;
	float jumpForce;
	
	bool   isFalling;
	double fallTime;

};