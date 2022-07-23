#pragma once
#include "Engine/Object/GameObject.h"

class PlatformRect : public GameObject
{
	enum class PlatformType
	{
		Texture,
		Animation
	};

public:
	// 텍스처 사용 안함
	PlatformRect(const D3DXVECTOR3& _size, const bool& _useCollider, Scene* _scene, GameObject* _parent);
	// 텍스처 사용
	PlatformRect(const D3DXVECTOR3& _size, const wstring& _texturePath, const bool& _useCollider, Scene* _scene, GameObject* _parent);
	// 애니메이션 사용
	PlatformRect(const D3DXVECTOR3& _size, const wstring& _texturePath, const size_t& _maxFrameX, const size_t& _maxFrameY, const bool& _useCollider, Scene* _scene, GameObject* _parent);
	~PlatformRect();

private:
	Animator* m_animator;
	Texture*  m_texture;
	AARect*   m_collider;

	D3DXVECTOR2 m_width;
	
};

