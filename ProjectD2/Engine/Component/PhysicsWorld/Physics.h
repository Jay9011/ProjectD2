#pragma once
class Physics
{
public:
	Physics();
	Physics(float jumpForce);

	D3DXVECTOR3 force;
	float mass;
	float jumpForce;
	
	bool   isFalling;
	double fallTime;	
};