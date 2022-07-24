#pragma once
#include "Engine/Component/PhysicsWorld/Physics.h"

class Gravity : public Component
{
private:
	Gravity(Physics& _physics, GameObject* _owner, int _updateOrder = 10);
	Gravity(Physics& _physics, float _maxFallSpeed, GameObject* _owner, int _updateOrder = 10);
	Gravity(Physics& _physics, float _maxFallSpeed, float _gravity, GameObject* _owner, int _updateOrder = 10);
	virtual ~Gravity() override;
	
public:
	// Component을(를) 통해 상속됨
	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void OnUpdateWorldTransform() override;

private:
	Physics& physics;
	float gravity;
	float maxFallSpeed;
	
	friend class Component;
};

