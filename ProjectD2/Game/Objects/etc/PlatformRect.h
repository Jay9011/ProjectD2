#pragma once
#include "Engine/Object/GameObject.h"

class PlatformRect : public GameObject
{
public:
	PlatformRect(const D3DXVECTOR3& _position, const D3DXVECTOR3& _size, Scene* _scene, GameObject* _parent);
	~PlatformRect();
};

