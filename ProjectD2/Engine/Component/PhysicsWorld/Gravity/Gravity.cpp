#include "Framework.h"
#include "Gravity.h"

#include "Engine/Object/GameObject.h"
#include "Engine/Component/Component.h"


Gravity::Gravity(Physics& _physics, GameObject* _owner, int _updateOrder) :
	Component(_owner, _updateOrder)
	, physics(_physics)
	, gravity(980)
	, maxFallSpeed(980)
{
#if _DEBUG
	TwAddVarRW(SCENE->GetCurrentScene()->twbar, "Gravity", TW_TYPE_FLOAT, &gravity, "step=1.0");
#endif // _DEBUG
}

Gravity::Gravity(Physics& _physics, float _maxFallSpeed, GameObject* _owner, int _updateOrder) :
	Gravity(_physics, _owner, _updateOrder)
{
	maxFallSpeed = _maxFallSpeed;
}

Gravity::Gravity(Physics& _physics, float _maxFallSpeed, float _gravity, GameObject* _owner, int _updateOrder) :
	Gravity(_physics, _maxFallSpeed, _owner, _updateOrder)
{
	gravity = _gravity;
}

Gravity::~Gravity()
{
}

void Gravity::Update()
{
	if (physics.isFalling)
	{
		physics.fallTime += fDT;

		if(physics.fallTime >= 0.2)
			physics.force.y += physics.mass * gravity * fDT;

		if(physics.force.y > maxFallSpeed)
		{
			physics.force.y = maxFallSpeed;
		}
	}
	else
	{
		physics.fallTime = 0;
	}
}

void Gravity::FinalUpdate()
{
}

void Gravity::OnUpdateWorldTransform()
{
}
