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
	// �ؽ�ó ��� ����
	PlatformRect(const D3DXVECTOR3& _size, const bool& _useCollider, Scene* _scene, GameObject* _parent);
	// �ؽ�ó ���
	PlatformRect(const D3DXVECTOR3& _size, const wstring& _texturePath, const bool& _useCollider, Scene* _scene, GameObject* _parent);
	// �ִϸ��̼� ���
	PlatformRect(const D3DXVECTOR3& _size, const wstring& _texturePath, const size_t& _maxFrameX, const size_t& _maxFrameY, const bool& _useCollider, Scene* _scene, GameObject* _parent);
	~PlatformRect();

private:
	Animator* m_animator;
	Texture*  m_texture;
	AARect*   m_collider;

	D3DXVECTOR2 m_width;
	
};

